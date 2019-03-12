# locust_test.py
# In python, '#' is used to indicate a comment line.
"""
The string within triple-quote is also considered as a comment.
And the triple-quote can be used for multiline comments.
DISCLAIMER: This sample doesn't care about whether the use of API is correct.
"""

import sys, random
from locust import HttpLocust, TaskSet

id_c = 0

def echo(locust):
    url_prefix = '/echo'
    locust.client.get('/echo', name = url_prefix)

def viewMeme(locust):
    postid = 1
    url_prefix = '/meme/view'
    locust.client.get('/meme/view?id='+ str(postid), name = url_prefix)

def createMeme(locust):
    global id_c
    id_c += 1
    url_prefix = '/meme/create';
    locust.client.get('/meme/create?image=' + str(id_c) + 'top=' + str(id_c) + 'bot=' + str(id_c), name=url_prefix)

def status(locust):
    """ define a function in python whose name is getList and the argument is locust """
    url_prefix = '/status'
    locust.client.get('/status', name = url_prefix)

def proxy_ucla(locust):
    """ define a function in python whose name is getList and the argument is locust """
    url_prefix = '/ucla'
    locust.client.get('/ucla', name = url_prefix)

class MyTaskSet(TaskSet):
    """ the class MyTaskSet inherits from the class TaskSet, defining the behavior of the user """
    tasks = {echo:1, viewMeme: 1, status: 1, proxy_ucla: 1}
    def on_start(locust):
        """ on_start is called when a Locust start before any task is scheduled """
        response = locust.client.get("/health")
        if response.status_code != 200:
            print("FAIL to start with posting data to server. Make sure that your server is running.")
            sys.exit();

class MyLocust(HttpLocust):
    """ the class MyLocust inherits from the class HttpLocust, representing an HTTP user """
    task_set = MyTaskSet
    min_wait = 1000
    max_wait = 2000
