(begin
	(define add (lambda (v) (+ v 10)))
	(define foo 100)
	(add foo)
	
	(if #f
		(add 200)
		(add 100))
)
