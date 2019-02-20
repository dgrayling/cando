(in-package :asdf-user)

(defsystem "cando-jupyter"
  :description "The CANDO compiler front end jupyter interface"
  :version "0.0.1"
  :author "Christian Schafmeister <chris.schaf@verizon.net>"
  :licence "LGPL-3.0"
  :depends-on (:inet
               :cando
               :cl-nglview
               :netcdf
               :static-vectors
               )
  :serial t
  :components
  ((:file "packages")
   (:file "cando-nglview")
   ))
