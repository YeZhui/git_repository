from django.conf.urls import url
from . import views

urlpatterns = [
    url('^login', views.login),
    url('^save_comm_day', view.saveCommDay),
    url('^show_comm_day', view.showCommDay),
    url('^save_diary', view.saveDiary),
    url('^show_diary', view.showDiary)
]