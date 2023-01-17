TARGET_BUCKET="ahc-cloud-run-jobs"
TARGET_FOLDER="marathon-sample"

gsutil -m cp -r upload_files/scripts gs://${TARGET_BUCKET}/${TARGET_FOLDER}/
