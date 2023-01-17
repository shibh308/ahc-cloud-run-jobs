FROM shibh308/cloud-sdk-with-cargo:1.1

ENV TARGET_BUCKET="ahc-cloud-run-jobs"
ENV TARGET_FOLDER="marathon-sample"

RUN apt-get update && apt-get install -y \
    zip \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

CMD gsutil -m cp -r gs://${TARGET_BUCKET}/${TARGET_FOLDER}/ ./ && \
    cd $TARGET_FOLDER && \
    chmod -R 755 ./ && \
    python3 scripts/run.py
