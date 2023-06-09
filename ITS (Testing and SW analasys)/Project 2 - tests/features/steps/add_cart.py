from behave import *
from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.common.action_chains import ActionChains
from selenium.webdriver.support import expected_conditions
from selenium.webdriver.support.wait import WebDriverWait
from selenium.webdriver.common.keys import Keys
from selenium.webdriver.common.desired_capabilities import DesiredCapabilities


@given('The product page is opened')
def step_impl(context):
    context.driver.get(
        'http://opencart:8080/en-gb/product/iphone?search=iphone'
    )
    context.driver.set_window_size(962, 1050)


@when('User clicks add to cart')
def step_impl(context):
    context.driver.find_element(By.ID, 'button-cart').click()


@then('The product is added to cart')
def step_impl(context):
    element = context.driver.find_element(By.ID, 'button-cart')
    actions = ActionChains(context.driver)
    actions.move_to_element(element).perform()
    element = context.driver.find_element(By.CSS_SELECTOR, 'body')
    actions = ActionChains(context.driver)
    actions.move_to_element(element).perform()
    context.driver.find_element(By.ID, 'top').click()
    context.driver.find_element(By.CSS_SELECTOR, '.btn-inverse').click()
