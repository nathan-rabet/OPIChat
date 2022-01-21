void swap(char *a, char *b)
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

void reverse(char str[], int len)
{
    int aller = 0;
    int retour = len - 1;
    while (aller < retour)
    {
        swap(&str[aller], &str[retour]);
        aller++;
        retour--;
    }
}

char *my_itoa(int value, char *s)
{
    int i = 0;
    int neg = 0;
    int res;
    if (value == 0)
    {
        s = "0";
        return s;
    }
    if (value < 0)
    {
        neg = 1;
        value = -value;
    }
    while (value != 0)
    {
        res = value % 10;
        s[i] = 48 + res;
        i++;
        value = value / 10;
    }
    if (neg)
    {
        s[i] = '-';
        i++;
    }
    s[i] = '\0';
    reverse(s, i);
    return s;
}
    