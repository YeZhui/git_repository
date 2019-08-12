from django.shortcuts import render
from django.http import HttpResponse
import requests
import logging
from . import models
# Create your views here.
logger = logging.getLogger('django')

def login(request):
    ret = {}
    ret['msgCode'] = 0
    code = request.GET['code']
    nickName = request.GET['nickName']
    if code == None:
        ret['msgCode'] = 99
        ret['msgInfo'] = '传入code为空'
        return HttpResponse(ret)
    params = {}
    params['appid'] = 'wx617432e7c4cada34'
    params['secret'] = '201ec39e5f086c8361478b40294a358c'
    params['js_code'] = code
    params['grant_type'] = 'authorization_code'
    urlStr = 'https://api.weixin.qq.com/sns/jscode2session'
    res = requests.get(urlStr, params = params)
    if res.status_code == 200:
        cust = models.cust_info()
        cust.cust_id = res.text.openid
        cust.nick_name = nickName
        cust.save()
        ret['msgInfo'] = res.text.openid
        return HttpResponse(ret)
    else:
        ret['msgCode'] = 99
        ret['msgInfo'] = '获取openid失败'
        return HttpResponse(ret)

#增加纪念日
def addCommDay(request):
    ret = {}
    ret['msgCode'] = 0
    commDate = request.GET['date']
    commDayFlag = request.GET['commDayFlag']
    commDayStr = request.GET['commDayStr']
    custId = request.GET['custId']
    if commDate == None:
        ret['msgCode'] = 99
        ret['msgInfo'] = '传入纪念日信息为空'
        return HttpResponse(ret)
    dateInfo = models.date_info()
    dateInfo.cust_id = custId
    dateInfo.record_date = commDate
    dateInfo.comm_day_flag = commDayFlag
    dateInfo.comm_day_info = commDayStr
    dateInfo.save()
    ret['msgInfo'] = '保存纪念日成功'
    return HttpResponse(ret)
    
#获取纪念日
def showCommDay(request):
    ret = {}
    ret['msgCode'] = 0
    custId = request.GET['custId']
    if custId == None:
        ret['msgCode'] = 99
        ret['msgInfo'] = '未传入客户标识，无法获取纪念日信息'
        return HttpResponse(ret)
    try:
        dateInfo = models.date_info.objects.filter(cust_id = custId)
    except:
        ret['msgInfo'] = '该用户不存在'
        return HttpResponse(ret)
    ret['dateInfo'] = dateInfo
    return HttpResponse(ret)

#保存日记
def addDiary(request):
    ret = {}
    ret['msgCode'] = 0
    custId = request.GET['custId']
    moodDate = request.GET['moodDate']
    moodWeather = request.GET['moodWeather']
    moodAddress = request.GET['moodAddress']
    moodState = request.GET['moodState']
    content = request.GET['content']
    todayMessage = request.GET['todayMessage']
    if custId == None:
        ret['msgCode'] = 99
        ret['msgInfo'] = '传入日记信息为空'
        return HttpResponse(ret)

    moodInfo = models.mood_info()
    moodInfo.cust_id = custId
    moodInfo.mood_date = moodDate
    moodInfo.mood_weather = moodWeather
    moodInfo.mood_address = moodAddress
    moodInfo.mood_state = moodState
    moodInfo.content = content
    moodInfo.today_message = todayMessage
    moodInfo.save()
    ret['msgInfo'] = '日记信息保存成功'
    return HttpResponse(ret)

#获取日记
def showDiary(request):
    ret = {}
    ret['msgCode'] = 0
    custId = request.GET['custId']
    if custId == None:
        ret['msgCode'] = 99
        ret['msgInfo'] = '未传入客户标识，无法获取日记信息'
        return HttpResponse(ret)
    try:
        moodInfo = models.mood_info.objects.filter(cust_id = custId)
    except:
        ret['msgInfo'] = '该用户不存在'
        return HttpResponse(ret)

    #返回列表
    ret['moodInfo'] = moodInfo
    return HttpResponse(ret)

#获取单条日记详细信息
def showDiaryInfo(request):
    ret = {}
    ret['msgCode'] = 0
    
