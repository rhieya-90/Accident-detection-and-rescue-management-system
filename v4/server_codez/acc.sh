while((1))
do
	a=$(mosquitto_sub -t project_14/x -C 1)
	echo x = $a >> data.txt;
	curl -v -X POST --data "{"x": $a }" https://demo.thingsboard.io/api/v1/j4bVqPwLLblfj45xNE02/telemetry --header "Content-Type:application/json"

	a=$(mosquitto_sub -t project_14/y -C 1)
	echo y = $a >> data.txt;
	curl -v -X POST --data "{"y": $a }" https://demo.thingsboard.io/api/v1/j4bVqPwLLblfj45xNE02/telemetry --header "Content-Type:application/json"

	a=$(mosquitto_sub -t project_14/z -C 1)
	echo z = $a >> data.txt;
	curl -v -X POST --data "{"z": $a }" https://demo.thingsboard.io/api/v1/j4bVqPwLLblfj45xNE02/telemetry --header "Content-Type:application/json"

done
