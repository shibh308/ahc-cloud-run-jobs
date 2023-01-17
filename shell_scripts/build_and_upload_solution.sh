TARGET_BUCKET="ahc-cloud-run-jobs"
TARGET_FOLDER="marathon-sample"

docker run --rm -v $(pwd):/app/ -it shibh308/cloud-sdk-with-cargo:1.1 bash -c "cd workspace && sh ../shell_scripts/build_solution_internal.sh"
gsutil -m cp -r upload_files/solution gs://${TARGET_BUCKET}/${TARGET_FOLDER}/
