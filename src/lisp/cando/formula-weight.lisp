(in-package :cando)


(defun formula-string-to-alist (formula)
  (loop with formula-alist = nil
        with rest-formula = formula
        with num = 1
        do (format t "rest-formula: ~s~%" rest-formula)
        do (let* ((element-symbol (chem:element-from-atom-name-string rest-formula))
                  (element-len (length (string element-symbol))))
             (setf rest-formula (subseq rest-formula element-len))
             (setf num (cond
                         ((= (length rest-formula) 0) 1) ; end of formula N=1
                         ((digit-char-p (aref rest-formula 0))
                          (let ((next-alpha-pos (position-if #'alpha-char-p rest-formula)))
                            (if (null next-alpha-pos)
                                (prog1
                                    (parse-integer rest-formula) ; last element - read N
                                  (setf rest-formula nil))
                                (prog1
                                    (parse-integer rest-formula :junk-allowed t) ;  - more formula - read N
                                  (setf rest-formula (subseq rest-formula next-alpha-pos))))))
                         (t 1)          ; No N - return 1 more formula
                         ))
             (when (eq element-symbol :ne)
               (error "Could not identify an element in formula ~a" formula))
             (push (cons element-symbol num) formula-alist)
             (format t "  at bottom: ~s - is empty ~a~%" rest-formula (string= rest-formula ""))
             (when (or (null rest-formula) (string= rest-formula ""))
               (return-from formula-string-to-alist (nreverse formula-alist))))))


(defgeneric formula-weight (formula))

(defmethod formula-weight ((formula cons))
  (loop for (element . number) in formula
        for atomic-weight = (chem:atomic-weight-for-element element)
        for weight = (* atomic-weight number)
        sum weight))
                                                            
  
(defmethod formula-weight ((formula-string string))
  (let ((formula (formula-string-to-alist formula-string)))
    (formula-weight formula)))
        
        
