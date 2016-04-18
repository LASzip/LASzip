#!/bin/bash

echo "deploying docs for $TRAVIS_BUILD_DIR/docs"
buildpath=`pwd`
if [[ ! -z $TRAVIS_BUILD_DIR ]]; then
buildpath="$TRAVIS_BUILD_DIR"
fi

docker run -v $buildpath:/data -w /data/docs laszip/docs aws s3 sync /docs/html/ s3://www.laszip.org --acl public-read --profile laszip --secret_key=$AWS_SECRET --access_key=$AWS_KEY
