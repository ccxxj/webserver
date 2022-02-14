#!/bin/bash

_term() {
	echo "Forwarding SIGTERM"
	kill -TERM "$child"
}

_int() {
	echo "Forwarding SIGINT"
	kill -INT "$child"
}

trap _term SIGTERM
trap _int SIGINT

./webserver &
child=$!

npm run test
res=$?
kill -TERM "$child"
exit $res
