from django.shortcuts               import render , redirect
from django.contrib.auth            import authenticate , login , logout
from django.contrib.auth.decorators import login_required 
from .form                          import login_form
from django.http                    import JsonResponse
# Create your views here.


def home(e):
    if e.user.is_authenticated:
        e.session['mode'] = '0'
        return render(e , 'core/home.html' , {})
    e.session.clear()
    return redirect('core:core_login')

def core_login(e):
    if e.method == "POST":
        form_from_post   = login_form(e.POST)
        if form_from_post.is_valid():
            data = form_from_post.cleaned_data
            user = authenticate( e , username = data['username'] ,password = data['password'])
            print( data['username'] , data['password'])
            if user is not None:
                login(request=e , user=user)
                return redirect('core:home')
        return redirect('core:core_login')
    return render(e , 'core/login.html' )

import requests
def core_logout(e):
    logout(e)
    return redirect("core:core_login")

def get_mode(e):
    if 'mode' not in e.session:
        e.session['mode'] = 0
    return JsonResponse({
        'mode':e.session['mode']
    })
    