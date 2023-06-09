from behave import *
from selenium.webdriver.common.action_chains import ActionChains
from selenium.webdriver.common.by import By
from selenium.webdriver.common.keys import Keys
from selenium.webdriver.support import expected_conditions
from selenium.webdriver.support.wait import WebDriverWait
from selenium.webdriver.common.desired_capabilities import DesiredCapabilities


@given('The store page is opened')
def step_impl(context):
    context.driver.get('http://opencart:8080/en-gb?route=common/home')
    context.driver.set_window_size(962, 1050)


@when('The user enters name of product in search bar')
def step_impl(context):
    context.driver.find_element(By.NAME, 'search').click()
    context.driver.find_element(By.NAME, 'search').send_keys('macbook')


@when('presses enter or clicks search')
def step_impl(context):
    context.driver.find_element(By.NAME, 'search').send_keys(Keys.ENTER)


@then('It displays products compatibile with the search quarie')
def step_impl(context):
    element = context.driver.find_element(
        By.CSS_SELECTOR, '.col:nth-child(1) > form button:nth-child(2)'
    )
    actions = ActionChains(context.driver)
    actions.move_to_element(element).perform()
    element = context.driver.find_element(By.CSS_SELECTOR, 'body')
    actions = ActionChains(context.driver)
    actions.move_to_element(element).perform()
    context.driver.find_element(By.ID, 'top').click()
    context.driver.find_element(By.CSS_SELECTOR, '.btn-inverse').click()
