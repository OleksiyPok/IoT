# AWS IoT Core

## Document Contents
- IoT Core configuration
- Examples of received and processed data

## IoT Core Configuration
### Roles

#### Roles overview

<img src="./images/AWS/Roles_01_.png" alt="AWS example data" width="700">

#### Roles in detail

#### iot_rule_db_role
<img src="./images/AWS/Roles_iot_rule_db_role_01_.png" alt="AWS example data" width="700">

#### iot_rule_cw_role
<img src="./images/AWS/Roles_iot_rule_cw_role_01_.png" alt="AWS example data" width="700">


---
### Roles Policies

#### Roles policies overview

<img src="./images/AWS/Policies_01_.png" alt="AWS example data" width="700">

#### Policies in detail

<img src="./images/AWS/Policies_02_.png" alt="AWS example data" width="700">


#### Policies in the JSON

##### Policies of "aws-iot-rule-StoreTelemetry-action-1-role-iot_rule_db_role"

```
{
    "Version": "2012-10-17",
    "Statement": {
        "Effect": "Allow",
        "Action": "dynamodb:PutItem",
        "Resource": "arn:aws:dynamodb:eu-central-1:873775216285:table/iot_telemetry"
    }
}
```  


##### Policies of "aws-iot-rule-StoreTelemetry-erroraction-role-iot_rule_cw_role"

```
{
    "Version": "2012-10-17",
    "Statement": [
        {
            "Effect": "Allow",
            "Action": [
                "logs:CreateLogStream",
                "logs:DescribeLogStreams",
                "logs:PutLogEvents"
            ],
            "Resource": [
                "arn:aws:logs:eu-central-1:873775216285:log-group:/aws/iot/rules/errors:*"
            ]
        }
    ]
}
```

##### Policies of "aws-iot-rule-state_alarms-action-1-role-iot_rule_cw_role"
```
{
    "Version": "2012-10-17",
    "Statement": [
        {
            "Effect": "Allow",
            "Action": [
                "logs:CreateLogStream",
                "logs:DescribeLogStreams",
                "logs:PutLogEvents"
            ],
            "Resource": [
                "arn:aws:logs:eu-central-1:873775216285:log-group:/aws/iot/rules/state_alarms:*"
            ]
        }
    ]
}
```


##### Policies of "aws-iot-rule-state_alarms-erroraction-role-iot_rule_cw_role"
```
{
    "Version": "2012-10-17",
    "Statement": [
        {
            "Effect": "Allow",
            "Action": [
                "logs:CreateLogStream",
                "logs:DescribeLogStreams",
                "logs:PutLogEvents"
            ],
            "Resource": [
                "arn:aws:logs:eu-central-1:873775216285:log-group:/aws/iot/rules/errors:*"
            ]
        }
    ]
}
```


##### Policies of "aws-iot-rule-temperature_alarm-action-1-role-iot_rule_cw_role"
```
{
    "Version": "2012-10-17",
    "Statement": [
        {
            "Effect": "Allow",
            "Action": [
                "logs:CreateLogStream",
                "logs:DescribeLogStreams",
                "logs:PutLogEvents"
            ],
            "Resource": [
                "arn:aws:logs:eu-central-1:873775216285:log-group:/aws/iot/rules/telemetry_alarms:*"
            ]
        }
    ]
}
```


##### Policies of "aws-iot-rule-temperature_alarm-erroraction-role-iot_rule_cw_role"
```
{
    "Version": "2012-10-17",
    "Statement": [
        {
            "Effect": "Allow",
            "Action": [
                "logs:CreateLogStream",
                "logs:DescribeLogStreams",
                "logs:PutLogEvents"
            ],
            "Resource": [
                "arn:aws:logs:eu-central-1:873775216285:log-group:/aws/iot/rules/errors:*"
            ]
        }
    ]
}
```


---
### Rules

#### Rules overview
<img src="./images/AWS/Rules_01_.png" alt="AWS example data" width="700">


#### Rule "StoreTelemetry" in detail
<img src="./images/AWS/Rules_StoreTelemetry_03_.png" alt="AWS example data" width="700">

##### SQL query for rule "StoreTelemetry"
```
SELECT *,
       timestamp() AS received_at,
       clientid()  AS client_id,
       topic(2)    AS student
FROM 'iot-course/OleksiiPok/telemetry'
WHERE bitand(sys_state, 2) = 0  //
  AND bitand(sys_state, 4) = 0 //
```

#### Rule "state_alarms" in detail
<img src="./images/AWS/Rules_state_alarms_03_.png" alt="AWS example data" width="700">

##### SQL query for rule "state_alarms" 
```
SELECT * 
FROM 'iot-course/OleksiiPok/telemetry' 
WHERE sys_state <> 0
```


#### Rule "temperature_alarm" in detail
<img src="./images/AWS/Rules_temperature_alarm_03_.png" alt="AWS example data" width="700">

##### SQL query for rule "temperature_alarm"
```
SELECT dht
FROM 'iot-course/OleksiiPok/telemetry'
WHERE bitand(sys_state, 2) = 0 
AND bitand(sys_state, 4) = 0
AND dht.temperature > 28
```


### Tables

<img src="./images/AWS/Tables_01_.png" alt="AWS example data" width="700">



### Logs

<img src="./images/AWS/Logs_01_.png" alt="AWS example data" width="700">



## Examples of received and processed data

### MQTT input messages

_Messages in the "Silent mode" and "Normal mode"_ 
<img src="./images/AWS/Mqtt_input_01_.png" alt="AWS example data" width="700">



### Tables

_DynamoDB > Explore items > iot_telemetry_
<img src="./images/AWS/Tables_03_.png" alt="AWS example data" width="700">



### Logs

#### "telemetry_alarms"
_CloudWatch > Log management > /aws/iot/rules/telemetry_alarms_
<img src="./images/AWS/Logs_telemetry_alarms_01_.png" alt="AWS example data" width="700">

_CloudWatch > Log management > /aws/iot/rules/telemetry_alarms (one)_
<img src="./images/AWS/Logs_telemetry_alarms_02_.png" alt="AWS example data" width="700">


#### "state_alarms"
_CloudWatch > Log management > /aws/iot/rules/state_alarms_
<img src="./images/AWS/Logs_state_alarms_01_.png" alt="AWS example data" width="700">

_CloudWatch > Log management > /aws/iot/rules/state_alarms (one)_
<img src="./images/AWS/Logs_state_alarms_02_.png" alt="AWS example data" width="700">


#### "errors"
_CloudWatch > Log management > /aws/iot/rules/errors_
<img src="./images/AWS/Logs_errors_01_.png" alt="AWS example data" width="700">

_CloudWatch > Log management > /aws/iot/rules/errors_ (one)
<img src="./images/AWS/Logs_errors_02_.png" alt="AWS example data" width="700">