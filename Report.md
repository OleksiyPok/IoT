# AWS IoT Core

## Document Contents
- IoT Core configuration
- Examples of received and processed data

## IoT Core Configuration
### Roles

#### Role overview

<img src="./images/AWS/Roles_01_.png" alt="AWS example data" width="700">

#### Role detail

#### iot_rule_db_role
<img src="./images/AWS/Roles_iot_rule_db_role_01_.png" alt="AWS example data" width="700">

#### iot_rule_cw_role
<img src="./images/AWS/Roles_iot_rule_cw_role_01_.png" alt="AWS example data" width="700">


---
### Role Policies

#### Role policies overview

<img src="./images/AWS/Policies_01_.png" alt="AWS example data" width="700">

#### Policy detail

<img src="./images/AWS/Policies_02_.png" alt="AWS example data" width="700">


#### Policies in JSON

##### Policy for "aws-iot-rule-StoreTelemetry-action-1-role-iot_rule_db_role"

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


##### Policy for "aws-iot-rule-StoreTelemetry-erroraction-role-iot_rule_cw_role"

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

##### Policy for "aws-iot-rule-state_alarms-action-1-role-iot_rule_cw_role"
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


##### Policy for "aws-iot-rule-state_alarms-erroraction-role-iot_rule_cw_role"
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


##### Policy for "aws-iot-rule-temperature_alarm-action-1-role-iot_rule_cw_role"
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


##### Policy for "aws-iot-rule-temperature_alarm-erroraction-role-iot_rule_cw_role"
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

#### Rule overview
<img src="./images/AWS/Rules_01_.png" alt="AWS example data" width="700">


#### Rule "StoreTelemetry" details
<img src="./images/AWS/Rules_StoreTelemetry_03_.png" alt="AWS example data" width="700">

##### SQL query for the "StoreTelemetry" rule
```
SELECT *,
       timestamp() AS received_at,
       clientid()  AS client_id,
       topic(2)    AS student
FROM 'iot-course/OleksiiPok/telemetry'
WHERE bitand(sys_state, 2) = 0  // SYSTEM_LDR_ERR
  AND bitand(sys_state, 4) = 0  // SYSTEM_DHT_ERR
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


### DynamoDB Tables

<img src="./images/AWS/Tables_01_.png" alt="AWS example data" width="700">



### CloudWatch Logs

<img src="./images/AWS/Logs_01_.png" alt="AWS example data" width="700">



## Examples of Received and Processed data

### MQTT Input Messages

*Messages in the "Silent mode" and "Normal mode"* 
<img src="./images/AWS/Mqtt_input_01_.png" alt="AWS example data" width="700">



### DynamoDB Tables

*DynamoDB > Explore items > iot_telemetry*
<img src="./images/AWS/Tables_03_.png" alt="AWS example data" width="700">



### CloudWatch Logs

#### "telemetry_alarms"
*CloudWatch > Log management > /aws/iot/rules/telemetry_alarms*
<img src="./images/AWS/Logs_telemetry_alarms_01_.png" alt="AWS example data" width="700">

*CloudWatch > Log management > /aws/iot/rules/telemetry_alarms (one)*
<img src="./images/AWS/Logs_telemetry_alarms_02_.png" alt="AWS example data" width="700">


#### "state_alarms"
*CloudWatch > Log management > /aws/iot/rules/state_alarms*
<img src="./images/AWS/Logs_state_alarms_01_.png" alt="AWS example data" width="700">

*CloudWatch > Log management > /aws/iot/rules/state_alarms (one)*
<img src="./images/AWS/Logs_state_alarms_02_.png" alt="AWS example data" width="700">


#### "errors"
*CloudWatch > Log management > /aws/iot/rules/errors*
<img src="./images/AWS/Logs_errors_01_.png" alt="AWS example data" width="700">

*CloudWatch > Log management > /aws/iot/rules/errors* (one)
<img src="./images/AWS/Logs_errors_02_.png" alt="AWS example data" width="700">