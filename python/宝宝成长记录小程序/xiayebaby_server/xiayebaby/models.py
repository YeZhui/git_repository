from django.db import models

# Create your models here.
class cust_info(models.Model):
    cust_id = models.CharField(max_length=64, primary_key=True, verbose_name=u"微信用户唯一标识")
    nick_name = models.CharField(max_length=64, verbose_name=u"用户昵称")
    cust_first_date = models.DateTimeField(verbose_name=u"客户注册时间",  auto_now_add=True) #auto_now_add 插入时默认，更新时不会
    cust_upd_date = models.DateTimeField(verbose_name=u"客户信息更新时间",  auto_now=True) #auto_now 只要有更新就会被强制更新为当前时间

    class Meta:
        db_table = 'cust_info'
        ordering = ['cust_first_date']
        verbose_name = '用户信息'
        verbose_name_plural = verbose_name

#保存用户记录心情
class mood_info(models.Model):
    mood_id = models.AutoField(primary_key=True,  verbose_name=u"心情标识")
    cust_id = models.CharField(max_length=64, verbose_name=u"用户标识")
    mood_date = models.IntegerField(verbose_name=u"日期")
    mood_weather = models.CharField(max_length=8, verbose_name=u"天气")
    mood_address = models.CharField(max_length=32, verbose_name=u"地点")
    mood_state = models.CharField(max_length=16, verbose_name=u"当前孕期")
    content = models.CharField(max_length=1024, verbose_name=u"今日心情")
    today_message = models.CharField(max_length=128, verbose_name=u"今日寄语")
    occur_date = models.DateTimeField(verbose_name=u"记录时间",  auto_now=True)

    class Meta:
        db_table = 'mood_info'
        ordering = ['cust_id', 'occur_date']
        verbose_name = '心情记录'
        verbose_name_plural = verbose_name

#保存纪念日
class date_info(models.Model):
    date_id = models.AutoField(primary_key=True,  verbose_name=u"纪念日标识")
    cust_id = models.CharField(max_length=64, verbose_name=u"用户标识")
    comm_day_flag = models.CharField(max_length=1, verbose_name=u"纪念日标识")
    comm_day_info = models.CharField(max_length=32, verbose_name=u"纪念日名称")
    record_date = models.IntegerField(verbose_name=u"纪念日日期")

    class Meta:
        db_table = 'date_info'
        ordering = ['cust_id']
        verbose_name = '纪念日'
        verbose_name_plural = verbose_name
    