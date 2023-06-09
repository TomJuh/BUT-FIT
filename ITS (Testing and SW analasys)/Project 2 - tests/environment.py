from selenium import webdriver
from selenium.webdriver.common.desired_capabilities import DesiredCapabilities
from selenium.webdriver.common.by import By
from selenium.common.exceptions import NoSuchElementException
from selenium.common.exceptions import NoAlertPresentException
from selenium.common.exceptions import WebDriverException


def get_webdriver():
    try:
        driver = webdriver.Remote(
            command_executor='http://localhost:4444/wd/hub',
            desired_capabilities=DesiredCapabilities.CHROME,
        )
    except WebDriverException:
        driver = webdriver.Remote(
            command_executor='http://localhost:4444/wd/hub',
            desired_capabilities=DesiredCapabilities.FIREFOX,
        )
    driver.implicitly_wait(1)
    return driver


def before_all(context):
    context.driver = get_webdriver()
    context.base_url = 'http://localhost:8080'


def after_all(context):
    context.driver.quit()
