target=en_decrypt ftp log network readini

all:
	for t in $(target);\
	do\
		cd "$$t" && make clean;\
		make "$$t" || exit "$$?";\
		cd -;\
	done

clean:
	for t in $(target);\
	do\
		cd "$$t" && make clean;\
		cd -;\
	done
