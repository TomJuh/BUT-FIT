# Installation

## Prerequisites

- NodeJS >= 16.0.0
- npm >= 8.0.0

Before any step in this guide:
```bash
# Install dependencies

$ npm i
```

## Run app without installing or generating build files

- Connect android device with enabled USB debug
- Or use local network to connect from the automatically opened local web
    (sometimes you need to use tunnel in connection options)

```bash
# Run application

$ npm run prod
```

## Build app

### Prerequisites
- JDK 8
- Android SDK for android build or MacOS for ios build
- Expo account https://expo.dev/

### Steps
```bash
# Install dependencies

$ npm i

# Build project

$ npm i -g turtle-cli

# build for android

$ turtle build:android -u EXPO_USERNAME -p EXPO_PASSWORD

# build for ios

$ turtle build:ios -u EXPO_USERNAME -p EXPO_PASSWORD
```

More about builds https://docs.expo.dev/classic/building-standalone-apps/

## Build app using expo servers
- waiting time is 2 - 30 min. depending on expo servers usage
- you can check servers usage at: https://expo.dev/turtle-status

### Prerequisites
- Expo account https://expo.dev/

### Steps
```bash

# build for android

$ npm run build:android

# build for ios

$ npm run build:ios

```

- After build is finished, you will receive link to download the build file (.apk for android)

# Develompent

```bash
# Install package only for development

$ npm i {PACKAGE_NAME} -D

# Install package for prod

$ npm i {PACKAGE_NAME} --save

# Run npm scripts

$ npm run {SCRIPT_NAME} # can be found in package.json
```

## Run development build locally

```bash
# Install package only for development

$ npm run android
```

## Style guides

- [Airbnb JS Style Guide](https://github.com/airbnb/javascript)
