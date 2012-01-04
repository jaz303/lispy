(begin
	(define add (lambda (v1 v2) (+ v1 v2)))
	
	(define make-adder (lambda (v1) (
		lambda (v2) (add v1 v2)
	)))
	
	(define foo 100)
	
	(if #f
		(add 200)
		(add 100))
		
	(define add-10 (make-adder 10))
	(define add-20 (make-adder 20))
	
	(add-20 foo)
)
