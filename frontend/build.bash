#!/bin/bash

npm run build
touch Staticfile
echo 'pushstate: enabled' >> build/Staticfile

