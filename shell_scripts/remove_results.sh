TARGET_BUCKET="ahc-cloud-run-jobs"
TARGET_FOLDER="marathon-sample"

gcloud storage rm -r gs://${TARGET_BUCKET}/${TARGET_FOLDER}/results/
