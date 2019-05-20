#!/usr/bin/python
# -*- coding: utf-8 -*-
__author__ = 'YeZhui Guan'

from django.db import models

# Create your models here.
class goodsType(models.Model):
    goods_type = models.CharField(max_length=1, unique=True, primary_key=True,  verbose_name=u"商品类别") 
    goods_type_name = models.CharField(max_length=32,  verbose_name=u"商品类别名称") 
    remark = models.CharField(max_length=64,  null=True,  verbose_name=u"备注")

    class Meta:
        db_table = 'goods_type'
        ordering = ['goods_type']  #排序
        verbose_name = '商品类别表'
        verbose_name_plural = '商品类别表'
        
    def __str__(self):
        return self.goods_type + '-' + self.goods_type_name
        
class goods(models.Model):
    goods_id = models.AutoField(primary_key=True,  verbose_name=u"商品标识")
    goods_type = models.CharField(max_length=1,  verbose_name=u"商品类别") 
    goods_name = models.CharField(max_length=32,  verbose_name=u"商品名称")
    goods_stock = models.IntegerField(verbose_name=u"商品库存")
    goods_price = models.DecimalField(max_digits=6,  decimal_places=2,  verbose_name=u"商品价格")
    goods_synopsis = models.CharField(max_length=64,  verbose_name=u"商品简介")
    goods_info = models.TextField(verbose_name=u"商品详情")
    
    class Meta:
        db_table = 'goods'
        ordering = ['goods_id']
        verbose_name = '商品表'
        
class goodsComments():
