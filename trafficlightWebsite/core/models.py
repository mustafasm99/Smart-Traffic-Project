from django.db import models
from django.contrib.auth.models import AbstractUser , BaseUserManager , User

class driver(AbstractUser):
    image            = models.ImageField(upload_to='profile_images/' , null=True , blank=True)
    phone_number     = models.CharField(max_length=15 )
    plate_number     = models.CharField(max_length=10)
    car_type         = models.CharField(max_length=20 , choices=[('Ambulance','سيارة اسعاف') , ('FireTrack' , 'سيارة اطفاء') , ('Cap',"سيارة شرطة")])
    city_name        = models.CharField(max_length=22)
    def get_all_requests(self):
        return  Request.objects.filter(user = self).all()
    

class Request(models.Model):
    user    = models.ForeignKey(driver , on_delete=models.SET_NULL , null=True)
    date    = models.DateField(auto_now_add=True)
    time    = models.TimeField(auto_now_add=True)
    update  = models.DateTimeField(auto_now_add=True)
    street  = models.CharField(max_length= 8 , choices=[
        ('s1','s1'),
        ('s2','s2'),
        ('s3','s3'),
        ('s4','s4'),
    ])

    def __str__(self)->str:
        return self.user.username + " " + str(self.date) + ":" + str(self.time)[:4]
    
