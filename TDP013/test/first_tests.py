from selenium import webdriver
from selenium.webdriver.common.keys import Keys

import time

driver = webdriver.Firefox()

driver.get('file:///home/sebastian/tdp013/src/index.html')

#write a couple of twidls
input = driver.find_element_by_id('twiput')
submit_button = driver.find_element_by_id('twibutt')

for i in range(1, 6):
    input.send_keys(i)
    submit_button.click()
    assert len(driver.find_elements_by_class_name('twidl')) == i #assert if tweet is posted

assert driver.find_elements_by_class_name('twidl')[0].find_element_by_tag_name('p').text == '5'
#read them bad boys
tweets = driver.find_elements_by_class_name('twidl')

for tweet in tweets:
    tweet.find_element_by_class_name('twidlcheck').click()
    assert float(tweet.value_of_css_property('opacity')) == float(0.3) #assert if read

#time to unread them in reverse order
for tweet in tweets[::-1]:
    tweet.find_element_by_class_name('twidlcheck').click()
    assert float(tweet.value_of_css_property('opacity')) == float(1) #assert if unread


#testing writing a empty "tweet"
input.send_keys("")
submit_button.click()
time.sleep(1)
driver.find_element_by_class_name('swal2-confirm').click()
assert len(driver.find_elements_by_class_name('twidl')) == 5 #assert that the empty tweet isn't added to tweets

input.send_keys("test"*36)
time.sleep(1)
submit_button.click()
time.sleep(1)
driver.find_element_by_class_name('swal2-confirm').click()
assert len(driver.find_elements_by_class_name('twidl')) == 5 #assert that the too long tweet isn't added to tweets

input.send_keys("refreshing")
time.sleep(.5)
input.send_keys(".")
time.sleep(.5)
input.send_keys(".")
time.sleep(.5)
#refreshing the page to "delete" the messages
driver.refresh()
assert len(driver.find_elements_by_class_name('twidl')) == 0 #assert that after refresh all content is gone

time.sleep(2)
driver.close()
