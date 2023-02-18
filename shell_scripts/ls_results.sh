TARGET_BUCKET="ahc-cloud-run-jobs"
TARGET_FOLDER="marathon-sample"

gcloud storage ls gs://${TARGET_BUCKET}/${TARGET_FOLDER}/results/ | wc -l