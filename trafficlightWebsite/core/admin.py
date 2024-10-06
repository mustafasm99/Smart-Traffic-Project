from django.contrib import admin
from django.contrib.auth.models import User
from django.contrib.auth.admin  import UserAdmin
# Register your models here.
from .models import (
    driver , 
    Request,
)

class manage_Request(admin.ModelAdmin):
    list_display    = ['id' , 'user' , 'street' ,'date' , 'time' , 'update']
    search_fields   = ['id' , 'user__username' , 'street']
    list_filter     = ['street']


# admin.site.unregister(User)
admin.site.register(driver)
admin.site.register(Request , manage_Request)