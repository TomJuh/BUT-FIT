import aiohttp
import asyncio
import re


async def get_urls(urls):
    async def get_url(url):
        async with aiohttp.ClientSession() as session:
            async with session.get(url) as resp:
                text = await resp.text()
                return resp.status

    async def get_urls(urls):
        results = []
        for url in urls:
            try:
                results.append(await asyncio.gather(get_url(url)))
            except Exception as e:
                results.append('aiohttp.ClientError')
        return results

    results = await asyncio.gather(get_urls(urls))
    results = results[0]
    for i in range(len(results)):
        results[i] = str(results[i])
        if results[i] != 'aiohttp.ClientError':
            results[i] = re.sub(r'\D', '', results[i])
            results[i] = int(results[i])

    end = []
    for i in range(len(urls)):
        end.append(tuple([results[i], urls[i]]))
    return list(end)


if __name__ == '__main__':

    urls = [
        'https://www.fit.vutbr.cz',
        'https://www.szn.cz',
        'https://www.alza.cz',
        'https://office.com',
        'https://aukro.cz',
    ]

    res = asyncio.run(get_urls(urls))
