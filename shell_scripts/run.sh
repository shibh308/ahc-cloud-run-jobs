if [ $1 = "tester" ]; then
	sh shell_scripts/build_and_upload_tester.sh
elif [ $1 = "solution" ]; then
	sh shell_scripts/build_and_upload_solution.sh
elif [ $1 = "scripts" ]; then
	sh shell_scripts/upload_scripts.sh
elif [ $1 = "execute" ]; then
	gcloud beta run jobs execute ahc-cloud-run-jobs
elif [ $1 = "clean" ]; then
	rm upload_files/tester/* upload_files/solution/*
fi
