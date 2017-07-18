for f in ./build/*.test
do
	valgrind --error-exitcode=42 --leak-check=full $f &> /dev/null
	rc=$?
	if [[ $rc == '42' ]]; then
		exit 1
	fi
done
