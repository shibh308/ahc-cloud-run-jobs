TARGET_BUCKET="ahc-cloud-run-jobs"
TARGET_FOLDER="marathon-sample"

gsutil -m cp -r gs://${TARGET_BUCKET}/${TARGET_FOLDER}/results/ $1