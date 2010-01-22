(import (rnrs)
        (mosh test))

(define (read-string str)
  (call-with-port (open-string-input-port str) read))

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

(test-equal "" (read-string "\"\\\n \""))
(test-equal "" (read-string "\"\\ \r \""))
(test-equal ""  (read-string "\"\\\t\r\t\""))

(test-results)
