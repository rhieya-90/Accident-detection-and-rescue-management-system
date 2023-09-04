while((1))
do
	a=$(mosquitto_sub -t project_14/accident -C 1)
	echo x = $a >> data.txt;
	curl -v -X POST --data "{"accident": $a }" https://demo.thingsboard.io/api/v1/j4bVqPwLLblfj45xNE02/telemetry --header "Content-Type:application/json" >> log_project.txt
done
