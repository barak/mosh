
;;------------------------------------------------
;; error handling
;;------------------------------------------------

; from library.scm

; overrides std raise
(define (raise c)
  (cond ((current-exception-handler)
	 => (lambda (proc)
	      (proc c)
	      (cond ((parent-exception-handler)
		     => (lambda (proc)
			  (let ((create-non-continuable-violation (symbol-value 'create-non-continuable-violation)))
			    (if create-non-continuable-violation
			      (proc (create-non-continuable-violation c))
			      (display "      create-non-continuable-violation is not set\n" (current-error-port)))))))
	      (throw "     error in raise(NMOSH): returned from non-continuable exception\n"))))
  (if %nmosh-failproc
    (%nmosh-failproc c (%get-stack-trace-obj))
    (throw (format "    error in raise(NMOSH): unhandled exception has occurred (and (nmosh startup) was not loaded)\n\n~a\n" 
                   ;(condition-printer c (current-error-port))
                   c
                   ))))

;;------------------------------------------------
;; error reporting
;;------------------------------------------------

;from Larceny
(define (raise-syntax-violation form subform who message syntax-form syntax-subform trace)
  (let* ((c0 (make-who-condition who))
	 (c1 (make-message-condition message))
	 (c2 (make-syntax-violation form subform))
	 (c3 (make-syntax-trace-condition syntax-form syntax-subform trace)) ; MOSH
	 (c (if who (condition c0 c1 c2 c3) (condition c1 c2 c3))))
    (raise c)))

#|
;deprecated
;;------------------------------------------------
;; condition objects used by VM and C procs
;;------------------------------------------------

(define-syntax setsym
  (syntax-rules () ((_ x) (set-symbol-value! (quote x) x))))

(setsym &condition)
(setsym &message)
(setsym &warning)
(setsym &serious)
(setsym &error)
(setsym &violation)
(setsym &assertion)
(setsym &irritants)
(setsym &who)
(setsym &non-continuable)
(setsym &implementation-restriction)
(setsym &lexical)
(setsym &syntax)
(setsym &undefined)
(setsym &i/o)
(setsym &i/o-read)
(setsym &i/o-write)
(setsym &i/o-invalid-position)
(setsym &i/o-filename)
(setsym &i/o-file-protection)
(setsym &i/o-file-is-read-only)
(setsym &i/o-file-already-exists)
(setsym &i/o-file-does-not-exist)
(setsym &i/o-port)
(setsym &i/o-decoding)
(setsym &i/o-encoding)
(setsym &no-infinities)
(setsym &no-nans)

(setsym &condition-rtd)
(setsym &condition-rcd)
(setsym &message-rtd)
(setsym &message-rcd)
(setsym &warning-rtd)
(setsym &warning-rcd)
(setsym &serious-rtd)
(setsym &serious-rcd)
(setsym &error-rtd)
(setsym &error-rcd)
(setsym &violation-rtd)
(setsym &violation-rcd)
(setsym &assertion-rtd)
(setsym &assertion-rcd)
(setsym &irritants-rtd)
(setsym &irritants-rcd)
(setsym &who-rtd)
(setsym &who-rcd)
(setsym &non-continuable-rtd)
(setsym &non-continuable-rcd)
(setsym &implementation-restriction-rtd)
(setsym &implementation-restriction-rcd)
(setsym &lexical-rtd)
(setsym &lexical-rcd)
(setsym &syntax-rtd)
(setsym &syntax-rcd)
(setsym &undefined-rtd)
(setsym &undefined-rcd)
(setsym &i/o-rtd)
(setsym &i/o-rcd)
(setsym &i/o-read-rtd)
(setsym &i/o-read-rcd)
(setsym &i/o-write-rtd)
(setsym &i/o-write-rcd)
(setsym &i/o-invalid-position-rtd)
(setsym &i/o-invalid-position-rcd)
(setsym &i/o-filename-rtd)
(setsym &i/o-filename-rcd)
(setsym &i/o-file-protection-rtd)
(setsym &i/o-file-protection-rcd)
(setsym &i/o-file-is-read-only-rtd)
(setsym &i/o-file-is-read-only-rcd)
(setsym &i/o-file-already-exists-rtd)
(setsym &i/o-file-already-exists-rcd)
(setsym &i/o-file-does-not-exist-rtd)
(setsym &i/o-file-does-not-exist-rcd)
(setsym &i/o-port-rtd)
(setsym &i/o-port-rcd)
(setsym &i/o-decoding-rtd)
(setsym &i/o-decoding-rcd)
(setsym &i/o-encoding-rtd)
(setsym &i/o-encoding-rcd)
(setsym &no-infinities-rtd)
(setsym &no-infinities-rcd)
(setsym &no-nans-rtd)
(setsym &no-nans-rcd)
|#
