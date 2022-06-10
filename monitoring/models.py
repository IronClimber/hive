from django.db import models
from django.utils import timezone

class Data(models.Model):
    pub_date = models.DateTimeField('pub_date', auto_now=True)

    ### Fan
    fan = models.IntegerField('fan')
    period = models.IntegerField('period')
    duration = models.IntegerField('duration')

    ### Audio
    audio1 = models.IntegerField('audio1')
    audio2 = models.IntegerField('audio2')
    audio3 = models.IntegerField('audio3')
    audio4 = models.IntegerField('audio4')

    ### Motion
    motion1 = models.IntegerField('motion1')
    motion2 = models.IntegerField('motion2')
    motion3 = models.IntegerField('motion3')
    motion4 = models.IntegerField('motion4')

    ### Humidity
    humidity1 = models.FloatField('humidity1')
    humidity2 = models.FloatField('humidity2')
    humidity3 = models.FloatField('humidity3')
    humidity4 = models.FloatField('humidity4')
    humidity5 = models.FloatField('humidity5')
    humidity6 = models.FloatField('humidity6')

    ### Temperature C
    tempc1 = models.FloatField('tempc1')
    tempc2 = models.FloatField('tempc2')
    tempc3 = models.FloatField('tempc3')
    tempc4 = models.FloatField('tempc4')
    tempc5 = models.FloatField('tempc5')
    tempc6 = models.FloatField('tempc6')
    ### Temperature F
    #tempf1 = models.FloatField('tempf1')
    #tempf2 = models.FloatField('tempf2')
    #tempf3 = models.FloatField('tempf3')
    #tempf4 = models.FloatField('tempf4')
    #tempf5 = models.FloatField('tempf5')
    #tempf6 = models.FloatField('tempf6')
    ### Heater Index C
    #hic1 = models.FloatField('hic1')
    #hic2 = models.FloatField('hic2')
    #hic3 = models.FloatField('hic3')
    #hic4 = models.FloatField('hic4')
    #hic5 = models.FloatField('hic5')
    #hic6 = models.FloatField('hic6')
    ### Heater Index F
    #hif1 = models.FloatField('hif1')
    #hif2 = models.FloatField('hif2')
    #hif3 = models.FloatField('hif3')
    #hif4 = models.FloatField('hif4')
    #hif5 = models.FloatField('hif5')
    #hif6 = models.FloatField('hif6')

class Fan(models.Model):
    period = models.IntegerField('period')
    duration = models.IntegerField('duration')

