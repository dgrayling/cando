(in-package :leap.solvate)


(defun tool-build-solute-array (matter)
  (let ((x-vec (make-array 10 :element-type 'double-float :fill-pointer 0 :adjustable t))
        (y-vec (make-array 10 :element-type 'double-float :fill-pointer 0 :adjustable t))
        (z-vec (make-array 10 :element-type 'double-float :fill-pointer 0 :adjustable t))
        (dx 0)
        (dy 0)
        (dz 0)
        (dxmax -1000000)
        (dymax -1000000)
        (dzmax -1000000)
        (dxmin 1000000)
        (dymin 1000000)
        (dzmin 1000000))
    (chem:map-atoms
     'nil
     (lambda (atom)
       (let ((pos (chem:get-position atom)))
         (setf dx (geom:vx pos)
               dy (geom:vy pos)
               dz (geom:vz pos))
         (vector-push-extend dx x-vec)
         (vector-push-extend dy y-vec)
         (vector-push-extend dz z-vec)
         (when (> dx dxmax)
           (setf dxmax dx))
         (when (> dy dymax)
           (setf dymax dy))
         (when (> dz dzmax)
           (setf dzmax dz))
         (when (< dx dxmin)
           (setf dxmin dx))
         (when (< dy dymin)
           (setf dymin dy))
         (when (< dz dzmin)
           (setf dzmin dz))))
     matter)
    (values x-vec y-vec z-vec (- dxmax dxmin) (- dymax dymin) (- dzmax  dzmin))))
         
;;; xstart,ystart,zstart is the CENTER of the first solvent box that goes at the max XYZ corner.
;;; ix,iy,iz is the number of solvent boxes
(defun tool-add-all-boxes (solute test-function solvent ix iy iz xstart ystart zstart xsolvent ysolvent zsolvent)
    (loop for i from 0 below ix
       for dx = xstart then (- dx xsolvent)
       do (loop for j from 0 below iy
             for dy = ystart then (- dy ysolvent)
             do (loop for k from 0 below iz
                   for dz = zstart then (- dz zsolvent)
                   for translate-solvent = (geom:vec dx dy dz)
                   for solvent-copy = (chem:matter-copy solvent)
                   for solvent-transform = (geom:make-m4-translate translate-solvent)
                   do (chem:apply-transform-to-atoms solvent-copy solvent-transform)
                   ;; Copy only solvent molecules that fit criteria
                   do (chem:map-molecules
                       nil
                       (lambda (solvent-mol)
                         ;; Check if solvent-mol satisfies criterion - if it does - add it to the solute
                         (when (funcall test-function solvent-mol)
                           (chem:add-matter solute solvent-mol))
                         )
                       solvent-copy)))))


;;Closeness controls how close solvent can get to solute before they are considered to be overlapping.
;;Farness defines shell's range.
(defun tool-solvate-and-shell (original-solute solvent width-list &key (closeness 0.0) (farness 10.0) shell oct isotropic verbose)
  (check-type width-list list)
  (let* ((solvent-box (chem:matter-get-property solvent :bounding-box))
         (solvent-x-width (first solvent-box))
         (solvent-y-width (second solvent-box))
         (solvent-z-width (third solvent-box))
         (atom-max-r (+ 1.5 closeness))
         solute-xvec solute-yvec solute-zvec
         solute-x-width solute-y-width solute-z-width
         xwidth ywidth zwidth
         ix iy iz
         xstart ystart zstart
         (solute (chem:matter-copy original-solute)))
    (let* ((solute-center (chem:geometric-center solute))
           (transform-to-origin (geom:make-m4-translate (geom:v* solute-center -1.0))))
      (chem:apply-transform-to-atoms solute transform-to-origin))
    (multiple-value-setq (solute-xvec solute-yvec solute-zvec solute-x-width solute-y-width solute-z-width)
      (tool-build-solute-array solute))   
    (setf xwidth (+ solute-x-width (* (first width-list) 2))
          ywidth (+ solute-y-width (* (second width-list) 2))
          zwidth (+ solute-z-width (* (third width-list) 2)))
    (if (or isotropic
            oct)
        (let ((widthtemp (* xwidth ywidth zwidth))
              (widthmax (max xwidth ywidth zwidth)))
          (setf xwidth widthmax
                ywidth widthmax
                zwidth widthmax
                widthtemp (/ (- (* widthmax widthmax widthmax) widthtemp) widthtemp))
          (format t "Total bounding box for atom centers:  ~a ~a ~a~%" xwidth ywidth zwidth)
          (format t "(box expansion for 'iso' is ~a )~%" (* 100.0 widthtemp))
          (setf widthtemp (max solute-x-width solute-y-width solute-z-width)
                solute-x-width widthtemp
                solute-y-width widthtemp
                solute-z-width widthtemp))
        (format t "Total bounding box for atom centers:  ~a ~a ~a~%" xwidth ywidth zwidth))
    (setf 
     ix (+ (truncate (/ xwidth solvent-x-width)) 1)
     iy (+ (truncate (/ ywidth solvent-y-width)) 1)
     iz (+ (truncate (/ zwidth solvent-z-width)) 1)
     xstart (* 0.5 solvent-x-width (- ix 1))
     ystart (* 0.5 solvent-y-width (- iy 1))
     zstart (* 0.5 solvent-z-width (- iz 1)))
    (when verbose
      (format t "xwidth ~a ywidth ~a zwidth ~a ix ~a iy ~a iz ~a xstart ~a ystart ~a zstart ~a~%" xwidth ywidth zwidth ix iy iz xstart ystart zstart))
    (let ((test-function (lambda (solvent-molecule)
                           (and (overlap-solvent solute-xvec solute-yvec solute-zvec solvent-molecule atom-max-r)
                                (if oct
                                    (invalid-solvent-octahedron xwidth ywidth zwidth solvent-molecule)
                                    (if shell
                                        (invalid-solvent-shell solute-xvec solute-yvec solute-zvec solvent-molecule farness)
                                        (invalid-solvent-rectangular xwidth ywidth zwidth solvent-molecule)))))))
      (tool-add-all-boxes solute test-function solvent ix iy iz xstart ystart zstart solvent-x-width solvent-y-width solvent-z-width)
      solute)))

(defun overlap-solvent (solute-xvec solute-yvec solute-zvec mol atom-max-r)
  (let* ((2xatom-max-r (* atom-max-r 2.0))
         (2xatom-max-r-squared (* 2xatom-max-r 2xatom-max-r)))
    (chem:map-atoms
     nil
     (lambda (a)
       (let ((solvent-pos (chem:get-position a)))
         (loop for i from 0 below (length solute-xvec)
            for x-solute = (aref solute-xvec i)
            for y-solute = (aref solute-yvec i)
            for z-solute = (aref solute-zvec i)
            for x-solvent = (geom:vx solvent-pos)
            for y-solvent = (geom:vy solvent-pos)
            for z-solvent = (geom:vz solvent-pos)
            for x-delta = (- x-solute x-solvent)
            for x-delta-squared = (* x-delta x-delta)
            for y-delta = (- y-solute y-solvent)
            for y-delta-squared = (* y-delta y-delta)
            for z-delta = (- z-solute z-solvent)
            for z-delta-squared = (* z-delta z-delta)
            do (when (< x-delta-squared 2xatom-max-r-squared)
                 (when (< y-delta-squared 2xatom-max-r-squared)
                   (when (< z-delta-squared 2xatom-max-r-squared)
                     (let ((delta-squared (+ x-delta-squared y-delta-squared z-delta-squared)))
                       (when (< delta-squared 2xatom-max-r-squared)
                         (return-from overlap-solvent nil)))))))))
     mol))
  t)

(defun invalid-solvent-rectangular (xwidth ywidth zwidth mol)
  (chem:map-atoms
   nil
   (lambda (a)
     (let ((solvent-pos (chem:get-position a)))
       (unless (and (> (/ xwidth 2.0) (abs (geom:vx solvent-pos)))
                    (> (/ ywidth 2.0) (abs (geom:vy solvent-pos)))
                    (> (/ zwidth 2.0) (abs (geom:vz solvent-pos))))
         (return-from invalid-solvent-rectangular nil))))
   mol)
  t)

(defun invalid-solvent-octahedron (xwidth ywidth zwidth mol)
  (let* ((width-avr (/ (+ xwidth ywidth zwidth) 3.0))
         (half-width-avr (/ width-avr 2.0))
         (2sqrt (sqrt 2.0))
         (2by3 (/ 2.0 3.0))
         (4by3 (/ 4.0 3.0))
         x-solvent y-solvent z-solvent)
    (chem:map-atoms
     nil
     (lambda (a)
       (let ((solvent-pos (chem:get-position a)))
         (setf x-solvent (geom:vx solvent-pos)
               y-solvent (geom:vy solvent-pos)
               z-solvent (geom:vz solvent-pos))
         (if (and (<= (- (* (* 2sqrt half-width-avr) 2by3)) z-solvent)
                  (>=  (* (* 2sqrt half-width-avr) 2by3) z-solvent))
             (progn                                 
               (when (>= x-solvent 0.0)
                 (if (and (<= (- (* 2sqrt x-solvent) (* 2sqrt half-width-avr)) z-solvent)
                          (>= (+ (* (- 2sqrt) x-solvent) (* 2sqrt half-width-avr)) z-solvent))
                     (progn
                       (when (>= y-solvent 0.0)
                         (if (and (<= (- (* 2sqrt y-solvent) (* 2sqrt half-width-avr)) z-solvent)
                                  (>= (+ (* (- 2sqrt) y-solvent) (* 2sqrt half-width-avr)) z-solvent)
                                  (>= (+ (- x-solvent) (* 4by3 half-width-avr)) y-solvent))
                             (return-from invalid-solvent-octahedron t)))
                       (when (< y-solvent 0.0)
                         (if (and (<= (- (* (- 2sqrt) y-solvent) (* 2sqrt half-width-avr)) z-solvent)
                                  (>= (+ (* 2sqrt y-solvent) (* 2sqrt half-width-avr)) z-solvent)
                                  (<= (- x-solvent (* 4by3 half-width-avr)) y-solvent))
                             (return-from invalid-solvent-octahedron t))))))
               (when (< x-solvent 0.0)
                 (if (and (<= (- (* (- 2sqrt) x-solvent) (* 2sqrt half-width-avr)) z-solvent)
                          (>= (+ (* 2sqrt x-solvent) (* 2sqrt half-width-avr)) z-solvent))
                     (progn
                       (when (>= y-solvent 0.0)
                         (if (and (<= (- (* 2sqrt y-solvent) (* 2sqrt half-width-avr)) z-solvent)
                                  (>= (+ (* (- 2sqrt) y-solvent) (* 2sqrt half-width-avr)) z-solvent)
                                  (>= (+ x-solvent (* 4by3 half-width-avr)) y-solvent))
                             (return-from invalid-solvent-octahedron t)))
                       (when (< y-solvent 0.0)
                         (if (and (<= (- (* (- 2sqrt) y-solvent) (* 2sqrt half-width-avr)) z-solvent)
                                  (>= (+ (* 2sqrt y-solvent) (* 2sqrt half-width-avr)) z-solvent)
                                  (<= (- (- x-solvent) (* 4by3 half-width-avr)) y-solvent))
                             (return-from invalid-solvent-octahedron t))))))))))
     mol))
  nil)

(defun invalid-solvent-shell (solute-xvec solute-yvec solute-zvec mol farness)
  (let* ((shell-radius farness)
         (2xshell-radius (* shell-radius shell-radius)))
    (chem:map-atoms
     nil
     (lambda (a)
       (let ((solvent-pos (chem:get-position a)))
         (loop for i from 0 below (length solute-xvec)
            for x-solute = (aref solute-xvec i)
            for y-solute = (aref solute-yvec i)
            for z-solute = (aref solute-zvec i)
            for x-solvent = (geom:vx solvent-pos)
            for y-solvent = (geom:vy solvent-pos)
            for z-solvent = (geom:vz solvent-pos)
            for x-delta = (- x-solute x-solvent)
            for x-delta-squared = (* x-delta x-delta)
            for y-delta = (- y-solute y-solvent)
            for y-delta-squared = (* y-delta y-delta)
            for z-delta = (- z-solute z-solvent)
            for z-delta-squared = (* z-delta z-delta)
            do (when (< x-delta-squared 2xshell-radius)
                 (when (< y-delta-squared 2xshell-radius)
                   (when (< z-delta-squared 2xshell-radius)
                     (let ((delta-squared (+ x-delta-squared y-delta-squared z-delta-squared)))
                       (when (< delta-squared 2xshell-radius)
                         (return-from invalid-solvent-shell t)))))))))
     mol))
  nil)

