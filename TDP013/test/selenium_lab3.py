from selenium import webdriver
from selenium.webdriver.common.keys import Keys

from pymongo import MongoClient as MC

import time

driver = webdriver.Firefox()
driver.get('file:///home/sebastian/tdp013/statics/src/index.html')

client = MC('mongodb://localhost:27017/test');
db = client["test"]["test"]

# clear collection before tests
db.drop()
# write a couple of twidls
input = driver.find_element_by_id('twiput')
submit_button = driver.find_element_by_id('twibutt')

for i in range(1, 6):
    input.send_keys(i)
    submit_button.click()
    # assert that tweet is posted
    assert len(driver.find_elements_by_class_name('twidl')) == i

# read them bad boys
tweets = driver.find_elements_by_class_name('twidl')

for i in range(len(tweets)-1):
    tweets[i].find_element_by_class_name('twidlcheck').click()
    # assert that tweet is read
    assert float(tweets[i].value_of_css_property('opacity')) == float(0.3)

#testing writing a empty "tweet"
input.send_keys("")
submit_button.click()
time.sleep(1)
driver.find_element_by_class_name('swal2-confirm').click()
# assert that the empty tweet isn't added to tweets
assert len(driver.find_elements_by_class_name('twidl')) == 5

input.send_keys("test"*36)
time.sleep(1)
submit_button.click()
time.sleep(1)
driver.find_element_by_class_name('swal2-confirm').click()
# assert that the too long tweet isn't added to tweets
assert len(driver.find_elements_by_class_name('twidl')) == 5

input.send_keys("refreshing")
time.sleep(.5)
input.send_keys(".")
time.sleep(.5)
input.send_keys(".")
time.sleep(.5)
#refreshing the page to "delete" the messages
driver.refresh()
time.sleep(.5)
# assert that after refresh all content is still there
assert len(driver.find_elements_by_class_name('twidl')) == 5

# read the last tweet after refreshing page
tweets = driver.find_elements_by_class_name('twidl')
tweets[(len(tweets)-1)].find_element_by_class_name('twidlcheck').click()
# assert that tweet is read
assert float(tweets[len(tweets)-1].value_of_css_property('opacity')) == float(0.3)

time.sleep(2)
db.drop()
driver.close()
