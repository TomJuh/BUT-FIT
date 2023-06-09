import React from 'react';

import LanguageListItem from '../components/LanguageListItem';
import {ScrollView} from 'native-base';
import {useSelector} from 'react-redux';

const languages = [
    {
        locale: 'cz',
        name: 'Čeština',
        englishName: 'Czech'
    },
    {
        locale: 'de',
        name: 'Deutsch',
        englishName: 'German'
    },
    {
        locale: 'en',
        name: 'English',
        englishName: 'English'
    },
    {
        locale: 'es',
        name: 'Español',
        englishName: 'Spanish'
    },
    {
        locale: 'fr',
        name: 'Français',
        englishName: 'French'
    },
    {
        locale: 'ru',
        name: 'Русский',
        englishName: 'Russian'
    },
    {
        locale: 'pl',
        name: 'Polski',
        englishName: 'Polish'
    },
    {
        locale: 'sk',
        name: 'Slovenčina',
        englishName: 'Slovak'
    },
    {
        locale: 'tu',
        name: 'Türkçe',
        englishName: 'Turkish'
    }
];

export default function LanguageSelectorScreen() {
    const {language: activeLanguageCode} = useSelector(state => state.settings);

    return (
        <ScrollView>
            {
                languages.map(({locale, name, englishName}) => (
                    <LanguageListItem
                        key={locale}
                        locale={locale}
                        name={name}
                        englishName={englishName}
                        isActive={locale === activeLanguageCode}
                    />
                ))
            }
        </ScrollView>
    );
}