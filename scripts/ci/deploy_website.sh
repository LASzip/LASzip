#!/bin/bash

echo "deploying docs for $TRAVIS_BUILD_DIR/docs"

export AWS_ACCESS_KEY_ID="$AWS_KEY"
export AWS_SECRET_ACCESS_KEY="$AWS_SECRET"

docker run -v $TRAVIS_BUILD_DIR:/data -w /data/docs laszip/docs aws s3 sync ./build/html/ s3://www.laszip.org --acl public-read
