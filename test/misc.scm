(import (rnrs)
        (mosh)
        (srfi :8)
        (system)
        (mosh test))

(define (read-string str)
  (call-with-port (open-string-input-port str) read))

(define (obj->fasl obj)
  (receive (port bv-proc) (open-bytevector-output-port)
    (fasl-write obj port)
    (bv-proc)))

(define (fasl->obj bv)
  (let ([port (open-bytevector-input-port bv)])
    (fasl-read port)))

(define (func a b c) c)


(test-equal "ABC\x0;ABC" (fasl->obj (obj->fasl (utf8->string #vu8(65 66 67 0 65 66 67)))))


(test-error assertion-violation? (vector-ref (vector) -1))
(test-error assertion-violation? (apply vector-ref (list (vector) -1)))
(test-error assertion-violation? (vector-set! (vector) -1 0))
(test-error assertion-violation? (apply vector-set! (list (vector) -1 0)))
(test-error assertion-violation? (vector-ref (vector) 'a))
(test-error assertion-violation? (apply vector-ref (list (vector) 'a)))

(test-error lexical-violation? (read-string "#(]"))
(test-error lexical-violation? (read-string "#vu8(]"))
(test-error lexical-violation? (read-string "(]"))
(test-error lexical-violation? (read-string "[)"))
(test-error lexical-violation? (read-string "#|#|"))

(test-equal "" (read-string "\"\\\n \""))
(test-equal "" (read-string "\"\\ \r \""))
(test-equal ""  (read-string "\"\\\t\r\t\""))

(test-equal #f
            (guard (c [#t #f])
                   (func 1)))


(test-equal "\nCONSTANT #t \nSYMBOL_P \nRETURN 0 \n"
            (call-with-string-output-port (lambda (p)
                                            (disasm (lambda () (symbol? #t)) p))))

(test-results)
