default : ex2 ex2t

ex2 : explomem2.c
	cc -o ex2 explomem2.c

ex2t : explomem2.c
	cc -o ex2t -DWITH_TEXT explomem2.c

clean :
	rm -f ex0 ex ex2 ex2t

