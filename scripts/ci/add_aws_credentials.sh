#!/bin/bash

openssl aes-256-cbc -K $encrypted_86ab07725c95_key -iv $encrypted_86ab07725c95_iv -in docs/aws-credentials.enc -out docs/aws-credentials -d

mkdir ~/.aws/
cp docs/aws-credentials ~/.aws/credentials


