from django.urls import path , include
from .views import *

app_name    = "core"

urlpatterns = [
    path('' , home , name="home"),
    path('core_login' , core_login , name="core_login"),
    path('core_logout' , core_logout , name="core_logout"),
    path('get_mode' , get_mode , name="get_mode")
]