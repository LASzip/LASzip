#!/bin/bash

echo "deploying docs for $TRAVIS_BUILD_DIR/docs"
buildpath=`pwd`
if [[ ! -z $TRAVIS_BUILD_DIR ]]; then
buildpath="$TRAVIS_BUILD_DIR"
fi

export AWS_ACCESS_KEY_ID="$AWS_KEY"
export AWS_SECRET_ACCESS_KEY="$AWS_SECRET"

docker run -v $buildpath:/data -w /data/docs laszip/docs aws s3 sync /data/docs/html/ s3://www.laszip.org --acl public-read
