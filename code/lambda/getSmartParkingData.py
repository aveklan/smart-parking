import boto3


def lambda_handler(event, context):
    dynamodb = boto3.resource('dynamodb')
    table = dynamodb.Table('smartparkTable')

    response = table.scan()

    items = response['Items']
    sorted_items = sorted(items, key=lambda x: x['timestamp'], reverse=True)

    return sorted_items
