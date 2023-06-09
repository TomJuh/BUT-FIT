from behave import *
import time
from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.common.action_chains import ActionChains
from selenium.webdriver.support import expected_conditions
from selenium.webdriver.support.wait import WebDriverWait
from selenium.webdriver.common.keys import Keys
from selenium.webdriver.common.desired_capabilities import DesiredCapabilities


@given('Chekcout page is opened')
def step_impl(context):
    context.driver.get('http://opencart:8080/en-gb?route=checkout/checkout')
    context.driver.set_window_size(962, 1050)
    context.driver.find_element(
        By.CSS_SELECTOR, '.form-check-inline:nth-child(2) > .form-check-label'
    ).click()


@given('all <required> fields have been filled in')
def step_impl(context):
    context.driver.find_element(By.ID, 'input-firstname').click()
    context.driver.find_element(By.ID, 'input-firstname').send_keys('first')
    context.driver.find_element(By.ID, 'input-lastname').send_keys('last')
    context.driver.find_element(By.ID, 'input-email').send_keys(
        'email@gmail.com'
    )
    context.driver.find_element(By.ID, 'input-shipping-address-1').send_keys(
        'addres1'
    )
    context.driver.find_element(By.ID, 'input-shipping-city').send_keys('city')
    context.driver.find_element(By.ID, 'input-shipping-postcode').send_keys(
        '000001'
    )
    element = context.driver.find_element(By.ID, 'input-shipping-country')
    actions = ActionChains(context.driver)
    actions.move_to_element(element).click_and_hold().perform()
    element = context.driver.find_element(By.ID, 'input-shipping-country')
    actions = ActionChains(context.driver)
    actions.move_to_element(element).perform()
    element = context.driver.find_element(By.ID, 'input-shipping-country')
    actions = ActionChains(context.driver)
    actions.move_to_element(element).release().perform()
    dropdown = context.driver.find_element(By.ID, 'input-shipping-country')
    context.driver.execute_script('window.scrollTo(0, 350);')
    time.sleep(2)

    dropdown = context.driver.find_element(By.ID, 'input-shipping-zone')
    dropdown.find_element(By.XPATH, "//option[. = 'Aberdeen']").click()
    element = context.driver.find_element(By.ID, 'input-shipping-zone')
    actions = ActionChains(context.driver)
    actions.move_to_element(element).click_and_hold().perform()
    element = context.driver.find_element(By.ID, 'input-shipping-zone')
    actions = ActionChains(context.driver)
    actions.move_to_element(element).perform()
    element = context.driver.find_element(By.ID, 'input-shipping-zone')
    actions = ActionChains(context.driver)
    actions.move_to_element(element).release().perform()


@when('User clicks on continue, fills in <required> delivery option')
def step_impl(context):
    context.driver.find_element(By.ID, 'button-register').click()
    context.driver.execute_script('window.scrollTo(0, -350);')

    dropdown = context.driver.find_element(By.ID, 'input-shipping-method')
    dropdown.find_element(
        By.XPATH, "//option[. = 'Flat Shipping Rate - $8.00']"
    ).click()
    dropdown = context.driver.find_element(By.ID, 'input-payment-method')
    dropdown.find_element(By.XPATH, "//option[. = 'Cash On Delivery']").click()


@when('clicks Confirm order')
def step_impl(context):
    context.driver.execute_script('window.scrollTo(0, 350);')
    time.sleep(2)
    context.driver.find_element(By.ID, 'button-confirm').click()


@then('the order is created')
def step_impl(context):
    wait = WebDriverWait(context.driver, 5)
    wait.until(
        lambda driver: driver.current_url
        == 'http://opencart:8080/en-gb?route=checkout/success'
    )
