from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC

from pymongo import MongoClient as MC

import time

DRIVER_URL = 'http://localhost:3000/'

driver = webdriver.Firefox()
driver.get(DRIVER_URL)

client = MC('mongodb://localhost:27017/test');
users = client["tests"]["users"]
messages = client["tests"]["messages"]
users.drop()
messages.drop()

def login_test(email, passw):
    driver.find_element_by_id('login').click() # go to loginpage

    time.sleep(0.5)

    email_inp = driver.find_element_by_id('signInEmail')
    pass_inp = driver.find_element_by_id('signInPassword')

    email_inp.send_keys(email)
    pass_inp.send_keys(passw)

    driver.find_element_by_id('rememberMe').click()
    driver.find_element_by_id('loginBtn').click()

    assert(driver.current_url == DRIVER_URL + '#/userpage')
    # Error testing

def fail_login_test():
    driver.find_element_by_id('login').click() # go to loginpage

    time.sleep(0.5)

    email_inp = driver.find_element_by_id('signInEmail')
    pass_inp = driver.find_element_by_id('signInPassword')

    email_inp.send_keys('doesnt@exist.com')
    pass_inp.send_keys('whatever')

    driver.find_element_by_id('loginBtn').click()

    assert(driver.title == 'Log in')

def logout_test():
    driver.find_element_by_id('logout').click()
    time.sleep(0.5)
    assert(driver.title == 'Log in')

def sign_up_test(email, passw, uname):
    driver.find_element_by_id('login').click() # go to loginpage

    time.sleep(0.5)

    driver.find_element_by_id('toSignUp').click()

    email_inp = driver.find_element_by_id('signUpEmail')
    pass_inp = driver.find_element_by_id('signUpPassword')
    user_np = driver.find_element_by_id('signUpUname')

    email_inp.send_keys(email)
    pass_inp.send_keys(passw)
    user_np.send_keys(uname)

    time.sleep(1)

    driver.find_element_by_id('rememberMe').click()
    driver.find_element_by_id('signUpBtn').click()

    time.sleep(0.5)

    assert(driver.current_url == DRIVER_URL + '#/userpage')

def fail_sign_up_test():
    driver.find_element_by_id('login').click() # go to loginpage

    time.sleep(0.5)

    driver.find_element_by_id('toSignUp').click()

    email_inp = driver.find_element_by_id('signUpEmail')
    pass_inp = driver.find_element_by_id('signUpPassword')
    user_np = driver.find_element_by_id('signUpUname')

    pass_inp.send_keys('whatever')

    driver.find_element_by_id('signUpBtn').click()

    assert(driver.title == 'Sign up')

def my_page_test(): # test after signUptest and login_test before login out
    time.sleep(0.5)
    user_info = driver.find_elements_by_tag_name('h2')

    assert(len(user_info) > 0)
    assert(driver.title == 'user_one')

def friend_page_test():
    personalInfo = WebDriverWait(driver, 10).until(
        EC.presence_of_element_located((By.ID, 'personalInfo'))
    )

    time.sleep(0.5)
    assert(driver.title == 'user_two')

    messageInput = driver.find_element_by_id('postInput')
    messageInput.send_keys('This is a test message!')
    driver.find_element_by_id('post-button').click()

    messages = WebDriverWait(driver, 10).until(
        EC.presence_of_all_elements_located((By.CLASS_NAME, 'post'))
    )

    assert(len(messages) == 1)

    #test input with zero and to many characters
    messageInput.clear()
    driver.find_element_by_id('post-button').click()

    danger_box = driver.find_element_by_class_name('alert-danger');

    assert(danger_box)

    messages = WebDriverWait(driver, 10).until(
        EC.presence_of_all_elements_located((By.CLASS_NAME, 'post'))
    )

    assert(len(messages) == 1)

    messageInput.clear()
    to_long_input = "a"*141
    messageInput.send_keys(to_long_input)
    driver.find_element_by_id('post-button').click()

    danger_box = driver.find_element_by_class_name('alert-danger');

    assert(danger_box)

    messages = WebDriverWait(driver, 10).until(
        EC.presence_of_all_elements_located((By.CLASS_NAME, 'post'))
    )

    assert(len(messages) == 1)

    #make friend
    driver.find_element_by_class_name('btn-light').click()
    driver.find_element_by_id('upage').click()

    friends = WebDriverWait(driver, 10).until(
        EC.presence_of_all_elements_located((By.CLASS_NAME, 'friend'))
    )

    assert(len(friends) == 1)

def friend_search_test():
    driver.find_element_by_id('search').click()

    # searching for friends
    search_bar = WebDriverWait(driver, 10).until(
        EC.presence_of_element_located((By.ID, 'emailSearch'))
    )
    search_bar.send_keys('user_two')
    driver.find_element_by_class_name('btn-primary').click()

    search_results = WebDriverWait(driver, 10).until(
        EC.presence_of_all_elements_located((By.CLASS_NAME, "resultMany"))
    )

    assert(len(search_results) == 2)

    # navigate to friendpage and run friend_page_test
    driver.find_element_by_class_name('btn-secondary').click()
    friend_page_test()

def home_test():
    driver.find_element_by_id('home').click()

    time.sleep(0.5)

    assert(driver.title == 'Home')


driver.find_element_by_id('logout').click()
time.sleep(1)
sign_up_test('c@mail.com', 'rty', 'user_two')
logout_test()
sign_up_test('b@mail.com', 'qwe', 'user_two')
logout_test()
sign_up_test('a@mail.com', 'asd', 'user_one')
my_page_test()
friend_search_test()
logout_test()
time.sleep(1)
login_test('a@mail.com', 'asd')
my_page_test()
home_test()
logout_test()
fail_sign_up_test()
fail_login_test()
time.sleep(1)

users.drop()
messages.drop()
driver.close()
