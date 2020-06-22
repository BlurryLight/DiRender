#!/usr/bin/env python
#encoding: utf-8

import requests
import argparse
import os
import sys

parser = argparse.ArgumentParser()
parser.add_argument("-t","--title",required=True,type=str,help='Message title')
parser.add_argument("-m","--message",default="",type=str,help='Message body')
args = parser.parse_args()

if('WECHAT_PUSH_API' in os.environ):
    API_KEY = os.environ.get('WECHAT_PUSH_API')
else:
    print("API KEY should be set as WECHAT_PUSH_API environment variable")
    sys.exit(-1)

api_url = "https://sc.ftqq.com/{0}.send".format(API_KEY)
msg = {"text": args.title, "desp": args.message}
req = requests.post(api_url,data=msg)
if(req.status_code == 200):
    print("Push successfully!")


