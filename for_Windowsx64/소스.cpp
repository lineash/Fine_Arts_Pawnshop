//
//  main.cpp
//  Fine_Arts_Pawnshop
//
//  Created by line.ash on 2021/06/04.
//
//��� ==========================================================================================================

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <bangtal.h>
#include <cstdlib>
#include <ctime>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAINROOM 10000
#define EVENTROOM 100
#define ENDROOM 1000
#define INVENTORY 4051
#define PAWNSHOP 4055
#define AUCTION 8000
#define NEXTDAYROOM 5840
#define SELL 0
#define BUY 1

//==============================================================================================================





//����ü ���� =====================================================================================================

struct works
{
    char name[100];
    char writer[100];
    char image_inventory[50], image_pawnshop[50], image_auction[50];
    float buy_price, sell_price, auction_price;
    bool original = true;
    bool have = false;
    int value;
};

struct people
{
    char image_pawnshop_idle[50], image_pawnshop_1[50], image_pawnshop_2[50], image_pawnshop_3[50], image_pawnshop_4[50], image_auction[50];
    struct works* slot[25];
    float intent;
    float deal_potential;
    int deal; // �ִ� 100 �ּ� 1. deal�� deal_potential��ŭ intent�� ���̰ų� �����.
    bool buy = false; // sell_price <= value * intent
    bool sell = false; // value * intent <= buy_price
    bool angry = false; //deal == 0
    int state = 3;
};

struct player
{
    float money;
    struct works* slot[5];
};

//==============================================================================================================





//�������� =======================================================================================================

struct player player;
struct works work[100];
struct people people[20];
SceneID startRoom, mainRoom, inventory, pawnshop, auction, storyRoom[30], eventRoom[30], endRoom[30], nextdayRoom;
ObjectID startButton, endButton, nextButton, inventory_backButton, pawnshop_backButton, auction_backButton, main_inventory, main_pawnshop, main_auction, offworkButton, gotoworkButton, inventory_slot[5], pawnshop_people, pawnshop_work, price_check, price_change, yesButton, noButton, dealButton;
ObjectID buy_idle, buy_over, buy_stop, exchange, sell_idle, sell_over1, sell_over2, sell_over3, sell_stop, pawnshop_state, wallet, drop[5], fact[5];
SoundID bgm, click;
TimerID showMoney;
int nowRoomNumber, day = 1, life = 3, workCount, peopleCount, consumerCount = 3;
float money = 10000000;
int buyorsell;
struct works* tempWork;
int tempI, tempRandom;
int fee;

//==============================================================================================================





//������Ʈ �����Լ� =================================================================================================

ObjectID createObject(const char* image, SceneID scene, int x, int y, bool shown = true)
{
    ObjectID object1 = createObject(image);
    locateObject(object1, scene, x, y);
    if (shown) showObject(object1);
    return object1;
}

//==============================================================================================================





//Ÿ�̸� �ݹ� =====================================================================================================

void timerCallback(TimerID timerF) //Ÿ�̸� �ݹ�
{

}

// =============================================================================================================





//��ǰ��� =====================================================================================================

void workEdit(char name[100], char writer[100], int value, bool original)
{
    for (int i = 0; i < 50; i++)
    {
        work[workCount].name[i] = name[i];
        work[workCount].writer[i] = writer[i];
    }
    sprintf_s(work[workCount].image_inventory, "image_inventory_%d.png", workCount);
    sprintf_s(work[workCount].image_pawnshop, "image_pawnshop_%d.png", workCount);
    sprintf_s(work[workCount].image_auction, "image_inventory_%d.png", workCount);
    work[workCount].value = value;
    work[workCount].original = original;
    workCount++;
}

// =============================================================================================================





//������ =======================================================================================================

void peopleEdit(float intent, float deal_potential, int deal)
{
    sprintf_s(people[peopleCount].image_pawnshop_idle, "%d_image_pawnshop_idle.png", peopleCount);
    /*
    sprintf(people[peopleCount].image_pawnshop_idle, "%d_image_pawnshop_1.png", peopleCount);
    sprintf(people[peopleCount].image_pawnshop_idle, "%d_image_pawnshop_2.png", peopleCount);
    sprintf(people[peopleCount].image_pawnshop_idle, "%d_image_pawnshop_3.png", peopleCount);
    sprintf(people[peopleCount].image_pawnshop_idle, "%d_image_pawnshop_4.png", peopleCount);
    */
    people[peopleCount].slot[0] = &work[peopleCount * 5];
    work[peopleCount * 5].have = true;
    people[peopleCount].slot[1] = &work[peopleCount * 5 + 1];
    work[peopleCount * 5 + 1].have = true;
    people[peopleCount].slot[2] = &work[peopleCount * 5 + 2];
    work[peopleCount * 5 + 2].have = true;
    people[peopleCount].slot[3] = &work[peopleCount * 5 + 3];
    work[peopleCount * 5 + 3].have = true;
    people[peopleCount].slot[4] = &work[peopleCount * 5 + 4];
    work[peopleCount * 5 + 4].have = true;
    people[peopleCount].intent = intent;
    people[peopleCount].deal_potential = deal_potential;
    people[peopleCount].deal = deal;
    peopleCount++;
}

// =============================================================================================================





//���丮 ���� �Լ� =================================================================================================

void sellExchange(int nextRoomNumber)
{
    char buf[30];
    sprintf_s(buf, "storyRoom_%d.png", nextRoomNumber);
    storyRoom[nextRoomNumber] = createScene(buf, buf);
    nextButton = createObject("nextButton.png", storyRoom[nextRoomNumber], 50, 50, true);
    scaleObject(nextButton, 0.5);
    enterScene(storyRoom[nextRoomNumber]);
    nowRoomNumber = nextRoomNumber;
}

// =============================================================================================================





//���丮 ���� �Լ� =================================================================================================

void story(int nextRoomNumber)
{
    char buf[30];
    sprintf_s(buf, "storyRoom_%d.png", nextRoomNumber);
    storyRoom[nextRoomNumber] = createScene(buf, buf);
    nextButton = createObject("nextButton.png", storyRoom[nextRoomNumber], 50, 50, true);
    scaleObject(nextButton, 0.5);
    enterScene(storyRoom[nextRoomNumber]);
    nowRoomNumber = nextRoomNumber;
}

// =============================================================================================================





//���� üũ �Լ� =================================================================================================

void lifeCheck()
{
    if (life == 0)
    {
        enterScene(eventRoom[1]);
        nowRoomNumber = EVENTROOM + 1;
    }
}

// =============================================================================================================





//���� üũ �Լ� =================================================================================================

void moneyCheck()
{
    if (money <= 0)
    {
        enterScene(eventRoom[2]);
        nowRoomNumber = EVENTROOM + 2;
    }
}

// =============================================================================================================





//�κ��丮 üũ �Լ� =================================================================================================

void inventoryCheck()
{
    for (int i = 0; i < 5; i++)
    {
        if (player.slot[i] != NULL)
        {
            setObjectImage(inventory_slot[i], player.slot[i]->image_inventory);
            //inventory_slot[i] = createObject(player.slot[i]->image_inventory, inventory, 100 + (i*225), 280, true);
            scaleObject(inventory_slot[i], 0.5);
        }
        if (player.slot[i] == NULL)
        {
            setObjectImage(inventory_slot[i], "inventory.png");
            //inventory_slot[i] = createObject("inventory.png", inventory, 100 + (i*225), 280, true);
            scaleObject(inventory_slot[i], 0.5);
        }
    }
}

// =============================================================================================================





//���콺 �ݹ� =====================================================================================================

void mouseCallback(ObjectID object, int x, int y, MouseAction action)
{
    playSound(click, false);
    if (object == startButton) //���ӽ��۹�ư, ���丮 ����
    {
        storyRoom[0] = createScene("storyRoom_0", "storyRoom_0.png");
        nextButton = createObject("nextButton.png", storyRoom[0], 50, 50, true);
        scaleObject(nextButton, 0.5);
        enterScene(storyRoom[0]);
        nowRoomNumber = 0;
    }
    else if (object == nextButton) //������ư, ���丮 ����
    {
        int step = nowRoomNumber;
        while (nowRoomNumber == step) //�ؽ�Ʈ ��ư�� �� ���ܾ� �Ѿ���� ��.
        {
            if (nowRoomNumber == 0) story(1);
            else if (nowRoomNumber == 1) story(2);
            else if (nowRoomNumber == 2) story(3);
            else if (nowRoomNumber == 3) story(4);
            else if (nowRoomNumber == 4) story(5);
            else if (nowRoomNumber == 5) story(6);
            else if (nowRoomNumber == 6) story(7);
            else if (nowRoomNumber == 7)
            {
                story(8);
                //showTimer(showMoney);
            }
            else if (nowRoomNumber == 8) story(9);
            else if (nowRoomNumber == 9) story(10);
            else if (nowRoomNumber == 10) story(11);
            else if (nowRoomNumber == 11) story(12);
            else if (nowRoomNumber == 12) story(13);
            else if (nowRoomNumber == 13)
            {
                enterScene(mainRoom);
                nowRoomNumber = MAINROOM;
            }
            else {

            }
        }
    }

    if (nowRoomNumber == MAINROOM) //���η� ������
    {
        lifeCheck();
        if (object == wallet)
        {
            char buf[100];
            sprintf(buf, "%d����\n������ %.0f���� �ֽ��ϴ�.\n����� ������ %d �Դϴ�.", day, money, life);
            showMessage(buf);
        }
        if (object == main_inventory)
        {
            enterScene(inventory);
            nowRoomNumber = INVENTORY;
            inventoryCheck();
        }

        if (object == main_pawnshop)//�ٽ��ۼ�
        {
            if (consumerCount == 0)
            {
                showMessage("������ ���̻� �մ��� ���� �ʴ´�.");
            }
            if (consumerCount > 0)
            {
                consumerCount--;
                int random = rand() % 5; //�մ� ����
                //int random = 0;
                int II = rand() % 5;
                if (II < 3)// �մ��� ��ǰ�� �ȷ� ��
                {
                    pawnshop_people = createObject(people[random].image_pawnshop_idle, pawnshop, 0, 330, true); //�մ� �����ֱ�
                    for (int i = 0; i < 1000; i++)
                    {
                        int randI = rand() % 10; //��ǰ����
                        if ((people[random].slot[randI] != NULL)) //�մ��� ���Կ� ��ǰ�� ������ && �̹� ������ ��ǰ�� �ƴϸ�
                        {
                            enterScene(pawnshop);
                            nowRoomNumber = PAWNSHOP;
                            setObjectImage(pawnshop_state, "sell_idle.png");
                            //������Ʈ �ӽ�����
                            tempWork = people[random].slot[randI];
                            tempI = randI;
                            tempRandom = random;
                            //�ʱ� ���Ű��� ����
                            people[random].slot[randI]->buy_price = (people[random].slot[randI]->value) * (people[random].intent);
                            //��ǰ�� �����ش�. ���� ������ƮID�� ���� ������Ʈ�� �����ư��� �����ϴ� �����̹Ƿ� ������ �߻��� �� ����.
                            pawnshop_work = createObject(people[random].slot[randI]->image_pawnshop, pawnshop, 300, 0, true);
                            yesButton = createObject("yesButton.png", pawnshop, 1050, 100, true);
                            scaleObject(yesButton, 0.5);
                            noButton = createObject("noButton.png", pawnshop, 1050, 40, true);
                            scaleObject(noButton, 0.5);
                            price_change = createObject("price_change.png", pawnshop, 850, 100, true);
                            scaleObject(price_change, 0.5);
                            dealButton = createObject("dealButton.png", pawnshop, 850, 40);
                            scaleObject(dealButton, 0.5);
                            buyorsell = BUY;
                            break;
                        }
                        else
                        {
                            showMessage("���� �մ��� ���� �ʾҽ��ϴ�.");
                            consumerCount++;
                            break;
                        }
                    }
                }
                if (II >= 3)//�մ��� ��ǰ�� �緯 ��
                {
                    pawnshop_people = createObject(people[random].image_pawnshop_idle, pawnshop, 0, 330, true); //�մ� �����ֱ�
                    //int randI = rand()%5;
                    for (int slotI = 0; slotI < 5; slotI++)
                    {
                        slotI += rand() % 5;
                        if (player.slot[slotI] != NULL) //�÷��̾��� ���Կ� ��ǰ�� ������
                        {
                            enterScene(pawnshop);
                            nowRoomNumber = PAWNSHOP;
                            setObjectImage(pawnshop_state, "buy_idle.png");
                            //������Ʈ �ӽ�����
                            tempWork = player.slot[slotI];
                            tempI = slotI;
                            tempRandom = random;
                            //�ʱ� �ǸŰ��� ���� //�����߻� people->player
                            int temp = (player.slot[slotI]->value) * (people[random].intent);
                            player.slot[slotI]->sell_price = (rand() % temp);
                            //��ǰ�� �����ش�. ���� ������ƮID�� ���� ������Ʈ�� �����ư��� �����ϴ� �����̹Ƿ� ������ �߻��� �� ����.
                            pawnshop_work = createObject(player.slot[slotI]->image_pawnshop, pawnshop, 300, 0, true);
                            yesButton = createObject("yesButton.png", pawnshop, 1050, 100, true);
                            scaleObject(yesButton, 0.5);
                            noButton = createObject("noButton.png", pawnshop, 1050, 40, true);
                            scaleObject(noButton, 0.5);
                            price_change = createObject("price_change.png", pawnshop, 850, 100, true);
                            scaleObject(price_change, 0.5);
                            dealButton = createObject("dealButton.png", pawnshop, 850, 40);
                            scaleObject(dealButton, 0.5);
                            setObjectImage(pawnshop_state, "buy_idle.png");
                            buyorsell = SELL;
                            //consumerCount--;
                            break;
                        }
                        else
                        {
                            showMessage("���� �մ��� ���� �ʾҽ��ϴ�.");
                            consumerCount++;
                            break;
                        }
                    }
                }
            }
        }
        /*
        if(object == main_pawnshop)
        {
            //Life �� ��ŭ �������� �մ� ����.
            if(consumerCount == 0) showMessage("������ ���̻� �մ��� ���� �ʴ´�.");
            if(consumerCount > 0)
            {
                consumerCount--;
                //int random = rand()%20; //�մ� ����
                int random = 0; //test
                int II = rand()%5;
                if (II < 3) //�մ��� ��ǰ�� �ȷ� �� ���
                {
                    bool avail = false;
                    for(int i = 0; i < 100 ; i++) //��ȿ�� �˻�
                    {
                        if (people[random].slot[i] != NULL) //������ ���� �ʾҴٸ�
                        {
                            avail = true;
                            break;
                        }
                    }

                    if(avail == true)
                    {
                        enterScene(pawnshop);
                        nowRoomNumber = PAWNSHOP;
                        pawnshop_people = createObject(people[random].image_pawnshop_idle, pawnshop, 0, 330, true); //�մ� �����ֱ�
                        for (int i = 0; i < 1000; i++)
                        {
                            int randI = rand()%10;
                            if((people[random].slot[randI] != NULL)) //�մ��� ���Կ� ��ǰ�� ������ && �̹� ������ ��ǰ�� �ƴϸ�
                            {
                                setObjectImage(pawnshop_state, "sell_idle.png");
                                //������Ʈ �ӽ�����
                                tempWork = people[random].slot[randI];
                                tempI = randI;
                                tempRandom = random;
                                //�ʱ� ���Ű��� ����
                                people[random].slot[randI]->buy_price = (people[random].slot[randI]->value)*(people[random].intent);
                                //��ǰ�� �����ش�. ���� ������ƮID�� ���� ������Ʈ�� �����ư��� �����ϴ� �����̹Ƿ� ������ �߻��� �� ����.
                                pawnshop_work = createObject(people[random].slot[randI]->image_pawnshop, pawnshop, 300, 0, true);
                                yesButton = createObject("yesButton.png", pawnshop, 1050, 100, true);
                                scaleObject(yesButton, 0.5);
                                noButton = createObject("noButton.png", pawnshop, 1050, 40, true);
                                scaleObject(noButton, 0.5);
                                price_change = createObject("price_change.png", pawnshop, 850, 100, true);
                                scaleObject(price_change, 0.5);
                                dealButton = createObject("dealButton.png", pawnshop, 850, 40);
                                scaleObject(dealButton, 0.5);
                                buyorsell = BUY;
                                //consumerCount--;
                                break;
                            }

                        }
                        //consumerCount--;
                        //break;
                    }
                    else if(avail == false)
                    {
                        showMessage("���� �մ��� �������� �ʾҽ��ϴ�.");
                        consumerCount++;
                        //break;
                    }
                }
                else if(II>3)//�մ��� ��ǰ�� �緯 �� ���
                {
                    pawnshop_people = createObject(people[random].image_pawnshop_idle, pawnshop, 0, 330, true); //�մ� �����ֱ�

                    for (int i = 0; i < 1000; i++)
                    {
                        bool avail = false;
                        for(int ii = 0; ii < 100; ii++) //��ȿ���˻�
                        {
                            if(player.slot[i] != NULL)
                                avail = true;
                        }
                        if(avail == true)
                        {
                            int randI = rand()%5;
                            if(player.slot[randI] != NULL) //�÷��̾��� ���Կ� ��ǰ�� ������
                            {
                                setObjectImage(pawnshop_state, "buy_idle.png");
                                //������Ʈ �ӽ�����
                                tempWork = player.slot[randI];
                                tempI = randI;
                                tempRandom = random;
                                //�ʱ� �ǸŰ��� ���� //�����߻� people->player
                                int temp = (player.slot[randI]->value)*(people[random].intent);
                                player.slot[randI]->sell_price = (rand()%temp);
                                //��ǰ�� �����ش�. ���� ������ƮID�� ���� ������Ʈ�� �����ư��� �����ϴ� �����̹Ƿ� ������ �߻��� �� ����.
                                pawnshop_work = createObject(player.slot[i]->image_pawnshop, pawnshop, 300, 0, true);
                                yesButton = createObject("yesButton.png", pawnshop, 1050, 100, true);
                                scaleObject(yesButton, 0.5);
                                noButton = createObject("noButton.png", pawnshop, 1050, 40, true);
                                scaleObject(noButton, 0.5);
                                price_change = createObject("price_change.png", pawnshop, 850, 100, true);
                                scaleObject(price_change, 0.5);
                                dealButton = createObject("dealButton.png", pawnshop, 850, 40);
                                scaleObject(dealButton, 0.5);
                                setObjectImage(pawnshop_state, "buy_idle.png");
                                buyorsell = SELL;
                                //consumerCount--;
                                break;//����
                            }
                        }
                        else if(avail == false)//�÷��̾��� ���Կ� ��ǰ�� ������ �մ��� �緯�� ������ �ٲ�
                        {


                            if(people[random].slot[i] != NULL) //�մ��� ���Կ� ��ǰ�� ������
                            {
                                //������Ʈ �ӽ�����
                                setObjectImage(pawnshop_state, "sell_idle.png");
                                tempWork = people[random].slot[i];
                                tempI = i;
                                tempRandom = random;
                                //�ʱ� ���Ű��� ����
                                people[random].slot[i]->buy_price = (people[random].slot[i]->value)*(people[random].intent);
                                //��ǰ�� �����ش�.
                                pawnshop_work = createObject(people[random].slot[i]->image_pawnshop, pawnshop, 300, 0, true);
                                yesButton = createObject("yesButton.png", pawnshop, 1050, 100, true);
                                scaleObject(yesButton, 0.5);
                                noButton = createObject("noButton.png", pawnshop, 1050, 40, true);
                                scaleObject(noButton, 0.5);
                                price_change = createObject("price_change.png", pawnshop, 850, 100, true);
                                scaleObject(price_change, 0.5);
                                dealButton = createObject("dealButton.png", pawnshop, 850, 40);
                                scaleObject(dealButton, 0.5);
                                buyorsell = BUY;
                                //consumerCount--;
                                break;

                            }
                        }
                    }
                    //consumerCount--;
                    //break;
                }
            }
        }*/
        if (object == main_auction)
        {
            enterScene(auction);
            nowRoomNumber = AUCTION;
        }
        if (object == offworkButton)
        {
            enterScene(nextdayRoom);
            nowRoomNumber = NEXTDAYROOM;
        }
    }
    if (nowRoomNumber == INVENTORY) //�κ��丮 ������
    {
        if (object == inventory_backButton)
        {
            enterScene(mainRoom);
            nowRoomNumber = MAINROOM;
        }
        for (int i = 0; i < 5; i++)
        {
            if (object == inventory_slot[i])
            {
                if (player.slot[i] == NULL)
                {
                    showMessage("�� �����Դϴ�.");
                }
                else
                {
                    char buf[100];
                    sprintf(buf, "��ǰ�� : %s \n �۰� : %s \n ���԰��� : %.0f��\n", player.slot[i]->name, player.slot[i]->writer, player.slot[i]->buy_price);
                    showMessage(buf);
                }

            }
        }
        if (object == drop[0])
        {
            player.slot[0] = NULL;
            inventoryCheck();
        }
        if (object == drop[1])
        {
            player.slot[1] = NULL;
            inventoryCheck();
        }
        if (object == drop[2])
        {
            player.slot[2] = NULL;
            inventoryCheck();
        }
        if (object == drop[3])
        {
            player.slot[3] = NULL;
            inventoryCheck();
        }
        if (object == drop[4])
        {
            player.slot[4] = NULL;
            inventoryCheck();
        }
        if (object == fact[0])
        {
            if (player.slot[0]->original == false)
            {
                showMessage("��ǰ�Դϴ�");
            }
            else {
                showMessage("��ǰ�Դϴ�");
            }
        }
        if (object == fact[1])
        {
            if (player.slot[1]->original == false)
            {
                showMessage("��ǰ�Դϴ�");
            }
            else {
                showMessage("��ǰ�Դϴ�");
            }
        }
        if (object == fact[2])
        {
            if (player.slot[2]->original == false)
            {
                showMessage("��ǰ�Դϴ�");
            }
            else {
                showMessage("��ǰ�Դϴ�");
            }
        }
        if (object == fact[3])
        {
            if (player.slot[3]->original == false)
            {
                showMessage("��ǰ�Դϴ�");
            }
            else {
                showMessage("��ǰ�Դϴ�");
            }
        }
        if (object == fact[4])
        {
            if (player.slot[4]->original == false)
            {
                showMessage("��ǰ�Դϴ�");
            }
            else {
                showMessage("��ǰ�Դϴ�");
            }
        }
    }
    if (nowRoomNumber == PAWNSHOP) //������ ������
    {
        if (object == pawnshop_backButton)
        {
            enterScene(mainRoom);
            nowRoomNumber = MAINROOM;
        }
        if (object == yesButton)
        {
            if (buyorsell == BUY)
            {
                if (money >= tempWork->buy_price)
                {
                    for (int i = 0; i < 5; i++)//�÷��̾� ���� üũ
                    {
                        if (player.slot[i] == NULL) //������ �������
                        {
                            money -= tempWork->buy_price;
                            player.slot[i] = tempWork; //�÷��̾��� ���Կ� ��ǰ�� �����ϰ�
                            people[tempRandom].slot[tempI]->have = true;
                            people[tempRandom].slot[tempI] = NULL; //�մ��� ���Կ��� ��ǰ�� ����
                            inventoryCheck(); //��ǰ ���
                            enterScene(inventory);
                            nowRoomNumber = INVENTORY;
                            fee += tempWork->buy_price;
                            break;
                        }
                    }

                }
                else
                {
                    showMessage("�ܾ��� �����մϴ�.");
                }
            }
            if (buyorsell == SELL)
            {
                money += tempWork->sell_price; //�ܾ��� ������Ű��
                player.slot[tempI]->have = true;
                player.slot[tempI] = NULL; //�÷��̾��� ���Կ��� ��ǰ�� ����
                for (int i = 0; i < 10; i++)
                {
                    if (people[tempRandom].slot[i] == NULL) //�մ��� �� ���Կ�
                    {
                        people[tempRandom].slot[i] = tempWork; //��ǰ�� ����
                        inventoryCheck();
                        enterScene(inventory);
                        if (people[tempRandom].slot[i]->original == false)
                        {
                            enterScene(eventRoom[0]);
                            nowRoomNumber = EVENTROOM + 0;
                        }
                        nowRoomNumber = INVENTORY;
                        fee += tempWork->sell_price;
                        break;
                    }
                }
            }
        }
        if (object == noButton) //�ŷ� ����
        {
            enterScene(mainRoom);
            nowRoomNumber = MAINROOM;
        }
        if (object == dealButton) //�ٽ� �����غ�
        {
            if (buyorsell == BUY)
            {
                int r = rand() % 100;
                int buf = (tempWork->buy_price) * people[tempRandom].deal_potential;
                //int buf = (people[tempRandom].slot[tempI]->buy_price)*people[tempRandom].deal_potential;
                //people[tempRandom].slot[tempI]->buy_price -= (rand()%100)/100*buf;
                tempWork->buy_price -= 0.01 * r * buf;
                hideObject(price_check);
                showObject(price_change);
                setObjectImage(pawnshop_state, "sell_over1.png");
                people[tempRandom].deal -= 1;
                if ((people[tempRandom].deal == 1) && rand() % 5 > 2)
                {
                    setObjectImage(pawnshop_state, "sell_over3.png");
                }
                if (people[tempRandom].deal == 0)
                {
                    nowRoomNumber = MAINROOM;
                    enterScene(mainRoom);
                    showMessage("�մ��� ȭ�� ���� �������ϴ�. ������ 1 �Ҿ����ϴ�.");
                    life--;
                    people[tempRandom].deal = 3;
                }
            }
            if (buyorsell == SELL)
            {
                int r = rand() % 100;
                int buf = ((player.slot[tempI]->sell_price) * people[tempRandom].deal_potential);
                player.slot[tempI]->sell_price += 0.01 * r * buf;
                hideObject(price_check);
                showObject(price_change);
                setObjectImage(pawnshop_state, "buy_over.png");
                people[tempRandom].deal -= 1;
                if ((people[tempRandom].deal == 1) && rand() % 5 > 2)
                {
                    setObjectImage(pawnshop_state, "sell_over3.png");
                }
                if (people[tempRandom].deal == 0)
                {
                    nowRoomNumber = MAINROOM;
                    enterScene(mainRoom);
                    showMessage("�մ��� ȭ�� ���� �������ϴ�. ������ 1 �Ҿ����ϴ�.");
                    life--;
                    lifeCheck();
                    people[tempRandom].deal = 3;
                }
            }
        }
        if (object == price_change)
        {
            if (buyorsell == BUY)
            {
                char buf[50];
                sprintf(buf, "%.0f��", tempWork->buy_price);
                showMessage(buf);
            }
            if (buyorsell == SELL)
            {
                char buf[50];
                sprintf(buf, "%.0f��", tempWork->sell_price);
                showMessage(buf);
            }
            hideObject(price_change);
            price_check = createObject("price_check.png", pawnshop, 850, 100, true);
            scaleObject(price_check, 0.5);
        }
        if (object == price_check)
        {
            if (buyorsell == BUY)
            {
                char buf[50];
                sprintf(buf, "%.0f��", tempWork->buy_price);
                showMessage(buf);
            }
            else if (buyorsell == SELL)
            {
                char buf[50];
                sprintf(buf, "%.0f��", tempWork->sell_price);
                showMessage(buf);
            }

        }
    }
    if (nowRoomNumber == AUCTION) //����� ������
    {
        if (object == auction_backButton)
        {
            enterScene(mainRoom);
            nowRoomNumber = MAINROOM;
        }

    }
    if (nowRoomNumber == NEXTDAYROOM) //������ ������
    {
        if (object == gotoworkButton)
        {
            enterScene(mainRoom);
            nowRoomNumber = MAINROOM;
            day++;
            consumerCount = life;
            money -= fee * 0.05;
            fee = 0;
            if (day % 7 == 0)
            {
                money -= 1000000;
            }
            if (day == 30)
            {
                money -= 10000000;
            }
            moneyCheck();
        }
    }
}

// =============================================================================================================





//���ӽ��� =======================================================================================================

SceneID game_init()
{
    startRoom = createScene("startRoom", "startRoom.png");
    startButton = createObject("startButton.png", startRoom, 50, 50, true);
    //endButton = createObject("endButton.png", startRoom, 300, 50, true);
    scaleObject(startButton, 0.5);
    //scaleObject(endButton, 0.5);
    bgm = createSound("bgm.mp3");
    click = createSound("click.mp3");
    playSound(bgm, true);

    //�⺻ ��, ��ư ����
    mainRoom = createScene("mainRoom", "mainRoom.png");
    main_inventory = createObject("inventory.png", mainRoom, 220, 280, true);
    scaleObject(main_inventory, 0.5);
    main_pawnshop = createObject("pawnshop.png", mainRoom, 560, 280, true);
    scaleObject(main_pawnshop, 0.5);
    main_auction = createObject("auction.png", mainRoom, 875, 280, true);
    scaleObject(main_auction, 0.5);
    offworkButton = createObject("offwork.png", mainRoom, 510, 50, true);
    scaleObject(offworkButton, 0.5);
    wallet = createObject("wallet.png", mainRoom, 875, 50, true);
    scaleObject(wallet, 0.5);

    nextdayRoom = createScene("nextdayRoom", "nextdayRoom.png");
    gotoworkButton = createObject("gotoworkButton.png", nextdayRoom, 510, 50, true);
    scaleObject(gotoworkButton, 0.5);

    inventory = createScene("inventory", "room_inventory.png");
    inventory_backButton = createObject("backButton.png", inventory, 50, 50, true);
    for (int i = 0; i < 5; i++)
    {
        inventory_slot[i] = createObject("inventory.png", inventory, 100 + (i * 225), 280, true);
        drop[i] = createObject("drop.png", inventory, 100 + (i * 225), 100, true);
        scaleObject(drop[i], 0.5);
        fact[i] = createObject("fact.png", inventory, 100 + (i * 225), 500, true);
        scaleObject(fact[i], 0.5);
    }
    scaleObject(inventory_backButton, 0.5);
    pawnshop = createScene("pawnshop", "room_pawnshop.png");
    pawnshop_backButton = createObject("backButton.png", pawnshop, 50, 50, true);
    pawnshop_state = createObject("sell_idle.png", pawnshop, 0, 150);
    scaleObject(pawnshop_backButton, 0.5);
    auction = createScene("auction", "room_auction.png");
    auction_backButton = createObject("backButton.png", auction, 50, 50, true);
    scaleObject(auction_backButton, 0.5);

    //����
    //������
    eventRoom[0] = createScene("eventRoom_0", "eventRoom_0.png");
    //�ذ�
    eventRoom[1] = createScene("eventRoom_1", "eventRoom_1.png");
    //�Ļ�
    eventRoom[2] = createScene("eventRoom_2", "eventRoom_2.png");

    //��ǰ ������ �Է�
    char work0[50] = "�𳪸���";
    char writer0[50] = "���������� �ٺ�ġ";
    workEdit(work0, writer0, 100000, false);

    char work1[50] = "�ڵ��ƺ��� ����";
    char writer1[50] = "����ī�� �̷γ��";
    workEdit(work1, writer1, 200000, false);

    char work2[100] = "���� �Ͱ��� �� �ҳ�";
    char writer2[50] = "���ϳ׽� �丣���̸�";
    workEdit(work2, writer2, 300000, false);

    char work3[50] = "ī���ϸ��� ����";
    char writer3[50] = "�Ͽ��� ȣ����";
    workEdit(work3, writer3, 400000, false);

    char work4[50] = "�����ϴ� ���";
    char writer4[50] = "���ͽ�Ʈ �δ�";
    workEdit(work4, writer4, 500000, false);

    //===================================================================
    //��ǰ ������ �Է�
    char work5[100] = "������ �̲��� ������ ����";
    char writer5[50] = "���� ���ũ���";
    workEdit(work5, writer5, 1000000, true);

    char work6[100] = "�׶�����Ʈ���� �Ͽ��� ����";
    char writer6[50] = "������ ���";
    workEdit(work6, writer6, 1100000, true);

    char work7[100] = "�������� �غ��� ���� �ĵ� �Ʒ�";
    char writer7[50] = "������ī ȣ�����";
    workEdit(work7, writer7, 800000, true);

    char work8[50] = "�ϳ��� �Ϻ�";
    char writer8[50] = "�轼 ����";
    workEdit(work8, writer8, 10000000, true);

    char work9[50] = "������ ����";
    char writer9[50] = "�߱� �þ�";
    workEdit(work9, writer9, 10000, false);

    //===================================================================
    //��ǰ ������ �Է�
    char work10[50] = "���� ������ ��";
    char writer10[50] = "��Ʈ �� ����";
    workEdit(work10, writer10, 10000000, true);

    char work11[50] = "�عٶ��";
    char writer11[50] = "��Ʈ �� ����";
    workEdit(work11, writer11, 500000, true);

    char work12[50] = "�𳪸���";
    char writer12[50] = "���������� �ٺ�ġ";
    workEdit(work12, writer12, 67000000, true);

    char work13[50] = "�̻� �ݴ� ���ε�";
    char writer13[50] = "���������� �з�";
    workEdit(work13, writer13, 6000000, true);

    char work14[50] = "������ ��";
    char writer14[50] = "�Ƴ�� ��Ŭ��";
    workEdit(work14, writer14, 1800000, true);

    //===================================================================
    //��ǰ ������ �Է�
    char work15[50] = "����� ������";
    char writer15[50] = "�� ����";
    workEdit(work15, writer15, 2000000, true);

    char work16[50] = "���� �������� ��";
    char writer16[50] = "���ξƸ� ����";
    workEdit(work16, writer16, 3500000, true);

    char work17[50] = "���� �׸޷���";
    char writer17[50] = "������ �ͳ�";
    workEdit(work17, writer17, 5000000, true);

    char work18[50] = "�� �Ѹ��� ���";
    char writer18[50] = "���������� �з�";
    workEdit(work18, writer18, 900000, true);

    char work19[50] = "�Ǹ��δ� �ҳ�";
    char writer19[50] = "���ξƸ� ����";
    workEdit(work19, writer19, 2000000, true);

    //===================================================================
    //��ǰ ������ �Է�
    char work20[50] = "�� ���� ����";
    char writer20[50] = "�����ý��� ���";
    workEdit(work20, writer20, 8000000, true);

    char work21[50] = "���ʸ���";
    char writer21[50] = "�� ������ �з���";
    workEdit(work21, writer21, 3400000, true);

    char work22[100] = "3�� ��Ÿ�� ������ ���ڿ�����";
    char writer22[50] = "�������̻�����";
    workEdit(work22, writer22, 700000, true);

    char work23[50] = "����";
    char writer23[50] = "��ȯ��";
    workEdit(work23, writer23, 100000000, true);

    char work24[50] = "Ȳ��";
    char writer24[50] = "���߼�";
    workEdit(work24, writer24, 50000000, true);
    /*
    //===================================================================
    //��ǰ ������ �Է�
    char work25[50] = "�𳪸���";
    char writer25[50] = "���������� �ٺ�ġ";
    workEdit(work25, writer25, 100000.0, false);

    char work26[50] = "�ڵ��ƺ��� ����";
    char writer26[50] = "����ī�� �̷γ��";
    workEdit(work26, writer26, 100000.0, false);

    char work27[50] = "�𳪸���";
    char writer27[50] = "���������� �ٺ�ġ";
    workEdit(work27, writer27, 100000.0, false);

    char work28[50] = "�𳪸���";
    char writer28[50] = "���������� �ٺ�ġ";
    workEdit(work28, writer28, 100000.0, false);

    char work29[50] = "�𳪸���";
    char writer29[50] = "���������� �ٺ�ġ";
    workEdit(work29, writer29, 100000.0, false);

    //===================================================================
    //��ǰ ������ �Է�
    char work30[50] = "�𳪸���";
    char writer30[50] = "���������� �ٺ�ġ";
    workEdit(work30, writer30, 100000.0, false);

    char work31[50] = "�ڵ��ƺ��� ����";
    char writer31[50] = "����ī�� �̷γ��";
    workEdit(work31, writer31, 100000.0, false);

    char work32[50] = "�𳪸���";
    char writer32[50] = "���������� �ٺ�ġ";
    workEdit(work32, writer32, 100000.0, false);

    char work33[50] = "�𳪸���";
    char writer33[50] = "���������� �ٺ�ġ";
    workEdit(work33, writer33, 100000.0, false);

    char work34[50] = "�𳪸���";
    char writer34[50] = "���������� �ٺ�ġ";
    workEdit(work34, writer34, 100000.0, false);

    //===================================================================
    //��ǰ ������ �Է�
    char work35[50] = "�𳪸���";
    char writer35[50] = "���������� �ٺ�ġ";
    workEdit(work35, writer35, 100000.0, false);

    char work36[50] = "�ڵ��ƺ��� ����";
    char writer36[50] = "����ī�� �̷γ��";
    workEdit(work36, writer36, 100000.0, false);

    char work37[50] = "�𳪸���";
    char writer37[50] = "���������� �ٺ�ġ";
    workEdit(work37, writer37, 100000.0, false);

    char work38[50] = "�𳪸���";
    char writer38[50] = "���������� �ٺ�ġ";
    workEdit(work38, writer38, 100000.0, false);

    char work39[50] = "�𳪸���";
    char writer39[50] = "���������� �ٺ�ġ";
    workEdit(work39, writer39, 100000.0, false);

    //===================================================================
    //��ǰ ������ �Է�
    char work40[50] = "�𳪸���";
    char writer40[50] = "���������� �ٺ�ġ";
    workEdit(work40, writer40, 100000.0, false);

    char work41[50] = "�ڵ��ƺ��� ����";
    char writer41[50] = "����ī�� �̷γ��";
    workEdit(work41, writer41, 100000.0, false);

    char work42[50] = "�𳪸���";
    char writer42[50] = "���������� �ٺ�ġ";
    workEdit(work42, writer42, 100000.0, false);

    char work43[50] = "�𳪸���";
    char writer43[50] = "���������� �ٺ�ġ";
    workEdit(work43, writer43, 100000.0, false);

    char work44[50] = "�𳪸���";
    char writer44[50] = "���������� �ٺ�ġ";
    workEdit(work44, writer44, 100000.0, false);

    //===================================================================
    //��ǰ ������ �Է�
    char work45[50] = "�𳪸���";
    char writer45[50] = "���������� �ٺ�ġ";
    workEdit(work45, writer45, 100000.0, false);

    char work46[50] = "�ڵ��ƺ��� ����";
    char writer46[50] = "����ī�� �̷γ��";
    workEdit(work46, writer46, 100000.0, false);

    char work47[50] = "�𳪸���";
    char writer47[50] = "���������� �ٺ�ġ";
    workEdit(work47, writer47, 100000.0, false);

    char work48[50] = "�𳪸���";
    char writer48[50] = "���������� �ٺ�ġ";
    workEdit(work48, writer48, 100000.0, false);

    char work49[50] = "�𳪸���";
    char writer49[50] = "���������� �ٺ�ġ";
    workEdit(work49, writer49, 100000.0, false);

    //===================================================================
    //��ǰ ������ �Է�
    char work50[50] = "�𳪸���";
    char writer50[50] = "���������� �ٺ�ġ";
    workEdit(work50, writer50, 100000.0, false);

    char work51[50] = "�ڵ��ƺ��� ����";
    char writer51[50] = "����ī�� �̷γ��";
    workEdit(work51, writer51, 100000.0, false);

    char work52[50] = "�𳪸���";
    char writer52[50] = "���������� �ٺ�ġ";
    workEdit(work52, writer52, 100000.0, false);

    char work53[50] = "�𳪸���";
    char writer53[50] = "���������� �ٺ�ġ";
    workEdit(work53, writer53, 100000.0, false);

    char work54[50] = "�𳪸���";
    char writer54[50] = "���������� �ٺ�ġ";
    workEdit(work54, writer54, 100000.0, false);

    //===================================================================
    //��ǰ ������ �Է�
    char work55[50] = "�𳪸���";
    char writer55[50] = "���������� �ٺ�ġ";
    workEdit(work55, writer55, 100000.0, false);

    char work56[50] = "�ڵ��ƺ��� ����";
    char writer56[50] = "����ī�� �̷γ��";
    workEdit(work56, writer56, 100000.0, false);

    char work57[50] = "�𳪸���";
    char writer57[50] = "���������� �ٺ�ġ";
    workEdit(work57, writer57, 100000.0, false);

    char work58[50] = "�𳪸���";
    char writer58[50] = "���������� �ٺ�ġ";
    workEdit(work58, writer58, 100000.0, false);

    char work59[50] = "�𳪸���";
    char writer59[50] = "���������� �ٺ�ġ";
    workEdit(work59, writer59, 100000.0, false);

    //===================================================================
    //��ǰ ������ �Է�
    char work60[50] = "�𳪸���";
    char writer60[50] = "���������� �ٺ�ġ";
    workEdit(work60, writer60, 100000.0, false);

    char work61[50] = "�ڵ��ƺ��� ����";
    char writer61[50] = "����ī�� �̷γ��";
    workEdit(work61, writer61, 100000.0, false);

    char work62[50] = "�𳪸���";
    char writer62[50] = "���������� �ٺ�ġ";
    workEdit(work62, writer62, 100000.0, false);

    char work63[50] = "�𳪸���";
    char writer63[50] = "���������� �ٺ�ġ";
    workEdit(work63, writer63, 100000.0, false);

    char work64[50] = "�𳪸���";
    char writer64[50] = "���������� �ٺ�ġ";
    workEdit(work64, writer64, 100000.0, false);

    //===================================================================
    //��ǰ ������ �Է�
    char work65[50] = "�𳪸���";
    char writer65[50] = "���������� �ٺ�ġ";
    workEdit(work65, writer65, 100000.0, false);

    char work66[50] = "�ڵ��ƺ��� ����";
    char writer66[50] = "����ī�� �̷γ��";
    workEdit(work66, writer66, 100000.0, false);

    char work67[50] = "�𳪸���";
    char writer67[50] = "���������� �ٺ�ġ";
    workEdit(work67, writer67, 100000.0, false);

    char work68[50] = "�𳪸���";
    char writer68[50] = "���������� �ٺ�ġ";
    workEdit(work68, writer68, 100000.0, false);

    char work69[50] = "�𳪸���";
    char writer69[50] = "���������� �ٺ�ġ";
    workEdit(work69, writer69, 100000.0, false);

    //===================================================================
    //��ǰ ������ �Է�
    char work70[50] = "�𳪸���";
    char writer70[50] = "���������� �ٺ�ġ";
    workEdit(work70, writer70, 100000.0, false);

    char work71[50] = "�ڵ��ƺ��� ����";
    char writer71[50] = "����ī�� �̷γ��";
    workEdit(work71, writer71, 100000.0, false);

    char work72[50] = "�𳪸���";
    char writer72[50] = "���������� �ٺ�ġ";
    workEdit(work72, writer72, 100000.0, false);

    char work73[50] = "�𳪸���";
    char writer73[50] = "���������� �ٺ�ġ";
    workEdit(work73, writer73, 100000.0, false);

    char work74[50] = "�𳪸���";
    char writer74[50] = "���������� �ٺ�ġ";
    workEdit(work74, writer74, 100000.0, false);

    //===================================================================
    //��ǰ ������ �Է�
    char work75[50] = "�𳪸���";
    char writer75[50] = "���������� �ٺ�ġ";
    workEdit(work75, writer75, 100000.0, false);

    char work76[50] = "�ڵ��ƺ��� ����";
    char writer76[50] = "����ī�� �̷γ��";
    workEdit(work76, writer76, 100000.0, false);

    char work77[50] = "�𳪸���";
    char writer77[50] = "���������� �ٺ�ġ";
    workEdit(work77, writer77, 100000.0, false);

    char work78[50] = "�𳪸���";
    char writer78[50] = "���������� �ٺ�ġ";
    workEdit(work78, writer78, 100000.0, false);

    char work79[50] = "�𳪸���";
    char writer79[50] = "���������� �ٺ�ġ";
    workEdit(work79, writer79, 100000.0, false);

    //===================================================================

    */

    //��� ������ �Է�
    peopleEdit(1.1, 0.1, 4); //0~4
    peopleEdit(1.3, 0.3, 3); //5~9
    peopleEdit(1.5, 0.2, 5); //10~14
    peopleEdit(1.6, 0.6, 2); //15~29
    peopleEdit(1.5, 0.3, 4); //20~24

    //test
    //player.slot[0] = &work[0];

    return startRoom;
}

//===============================================================================================================





//���� ===========================================================================================================

int main()
{
    srand((unsigned)time(NULL));
    //�ݹ��Լ�
    setMouseCallback(mouseCallback);
    setTimerCallback(timerCallback);

    //���� �������̽� ó��
    setGameOption(GameOption::GAME_OPTION_ROOM_TITLE, false);
    setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, false);
    setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, false);

    //Ÿ�̸ӵ��
    showMoney = createTimer(money);
    setTimer(showMoney, money);
    startGame(game_init());
}

//===============================================================================================================
