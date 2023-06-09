def create_tuple(answers, questions):
    merged_list = []
    for i in range(max((len(answers), len(questions)))):

        while True:
            try:
                tup = (answers[i], questions[i])
            except IndexError:
                break
            merged_list.append(tup)
            break
    return merged_list


def gen_quiz(
    qpool, *indexes, altcodes=('A', 'B', 'C', 'D', 'E', 'F'), quiz=None
):

    Questions = []
    Answers = []
    newItems = []

    if quiz is None:
        quiz = []
    for i in indexes:
        try:
            Questions = qpool[i][0]
            Answers = qpool[i][1]
        except Exception as execE:
            print('Ignoring index ' + str(i) + ' - ' + str(execE))
            continue
        for items in create_tuple(altcodes, Answers):
            newItems.append(str(items[0]) + ': ' + str(items[1]))
        quiz.append((Questions, newItems))
        newItems = []
    return quiz
