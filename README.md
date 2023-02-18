# ahc-cloud-run-jobs


## 概要
shibh308のマラソン環境の雛形, Google Cloud RunのJob機能で手元のプログラムを並列実行できる


## 使い方

### 事前準備
テストケース
- `/upload_files/tester` 以下にテストケースを入れておく
  - デフォルトではテストケースをフォルダごと `in.zip` として保存している

変数の設定
- `scripts/*`, `shell_scripts/*`, `Dockerfile` の `TARGET_BUCKET`, `TARGET_FOLDER` を変更しておく

実行環境の設定
- `shell_scripts/*_internal.sh` の内容を書き換えておく（Rustを使ったりテスターの形式が異なる場合 ）

コンテナのビルド・アップロード
- このファイルのあるディレクトリで `docker build` してArtifact Registry等にアップロードしておく

ジョブの生成
- `gcloud beta run jobs create ahc-cloud-run-jobs --image IMAGE_NAME --tasks NUM_TASKS` でジョブ生成
  - 同名ジョブがある場合は `create` が `update` になるので注意


### 実行
ファイルのアップロード
- `sh shell_scripts/run.sh tester`
  - ツール類のビルドとアップロード
- `sh shell_scripts/run.sh solution`
  - 解答コードのビルドとアップロード
- `sh shell_scripts/run.sh scripts`
  - ジョブ内で動かすスクリプト（ `uploads/scripts/` 以下）のアップロード

ジョブの実行
- `sh shell_scripts/run.sh execute`
  - アップロードされたファイルを元に並列実行
- `python3 scripts/collect_scores.py`
  - 実行と集計をまとめて行う
