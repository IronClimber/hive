from django.shortcuts import render
from django.http import HttpResponse
from .models import Data, Fan
from django.views.decorators.csrf import csrf_exempt
from .forms import DataForm
from django.utils import timezone
import csv

period = 100
duration = 20

def index(request):
    data_list = Data.objects.order_by('-pub_date') 
    return render(request, 'monitoring/charts.html',{'data_list':data_list})

def get_duration(request):
    if request.method == 'GET':
        return HttpResponse(duration)

def get_last_objects(request,value):
    data_list = Data.objects.order_by('-pub_date')[:value]
    return render(request, 'monitoring/charts.html',{'data_list':data_list})

def get_period(request):
    if request.method == 'GET':
        return HttpResponse(period)

def set_duration(request, value):
    if (value>0 and value<=60 and value<period):
        global duration
        duration = value
        return HttpResponse("duration=%s" % duration)
    else:
        return HttpResponse("Wrong value: %s \r\nShould be in range (0,60]" % value)

def set_period(request, value):
    if (value>0 and value<=120 and value>duration):
        global period
        period = value
        return HttpResponse("period=%s" % period)
    else:
        return HttpResponse("Wrong value: %s \r\nShould be in range (0,120]" % value)

def get_csv(request):
    # Create the HttpResponse object with the appropriate CSV header.
    response = HttpResponse(
        content_type='text/csv',
        headers={'Content-Disposition': 'attachment; filename="Data%s.csv"' % timezone.localtime().strftime("%Y-%m-%D-%H-%M-%S")})

    fields = [f.name for f in Data._meta.fields]
    writer = csv.writer(response)
    writer.writerow(fields)

    for obj in Data.objects.all():
        row = [str(getattr(obj, field)) for field in fields]
        #for field in fields:
        #     row += str(getattr(obj, field)) + ","
        writer.writerow(row)


    return response

@csrf_exempt
def publish(request):
    #data_set = Data(pub_date=timezone.now())
    if request.method == 'POST':
        #return HttpResponse(request)
        form = DataForm(request.POST)
        #body_unicode = request.body.decode('utf-8')
        if form.is_valid():
            form.save()
            return HttpResponse("duration=%s&period=%s" % (duration, period))
        #a = ""
        #for header in request.headers: 
        #    a+=header
        #    a+=": "
        #    a+=request.headers[header]

        #return HttpResponse(a)
        #return HttpResponse(request.headers)
        return HttpResponse(form.errors)

    return HttpResponse("neе POST")
