all:
	gcc reverse.c libsndfile-1.0.26/src/*.o libsndfile-1.0.26/src/ALAC/*.o libsndfile-1.0.26/src/G72x/*.o libsndfile-1.0.26/src/GSM610/*.o -Ilibsndfile-1.0.26/src -lm -o reverse
