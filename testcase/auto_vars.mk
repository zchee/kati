# TODO(c-ninja): Fix

test1: foo bar foo
	echo $<
	echo $@
	echo $^
	echo $+

foo: baz
	echo $<

bar:
	echo $<

baz:

test2: foo bar foo
	echo $^
	echo $+
