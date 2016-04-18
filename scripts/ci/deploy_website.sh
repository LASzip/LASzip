#!/bin/bash

aws s3 sync "$1"/html/ s3://www.laszip.org --acl public-read --profile laszip
