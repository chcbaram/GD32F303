/*
 * usb.c
 *
 *  Created on: 2021. 11. 14.
 *      Author: baram
 */




#include "usb.h"
#include "cdc.h"
#include "cli.h"

#ifdef _USE_HW_USB
#include "usbd_core.h"

#if HW_USE_CDC == 1
#include "cdc_acm_core.h"
#endif

#if HW_USE_MSC == 1
#include "usbd_msc.h"
#include "usbd_storage_if.h"
#endif


#include "usbd_lld_int.h"

static bool is_init = false;
static UsbMode_t is_usb_mode = USB_NON_MODE;

usb_dev usbd_cdc;



#ifdef _USE_HW_CLI
static void cliCmd(cli_args_t *args);
#endif






bool usbInit(void)
{
  bool ret = true;
  uint32_t system_clock = rcu_clock_freq_get(CK_SYS);


  // enable USB pull-up pin clock
  //
  rcu_periph_clock_enable(RCC_AHBPeriph_GPIO_PULLUP);

  if (48000000U == system_clock)
  {
    rcu_usb_clock_config(RCU_CKUSB_CKPLL_DIV1);
  } else if (72000000U == system_clock)
  {
    rcu_usb_clock_config(RCU_CKUSB_CKPLL_DIV1_5);
  } else if (96000000U == system_clock)
  {
    rcu_usb_clock_config(RCU_CKUSB_CKPLL_DIV2);
  } else if (120000000U == system_clock)
  {
    rcu_usb_clock_config(RCU_CKUSB_CKPLL_DIV2_5);
  } else
  {
    /* reserved */
  }

  // enable USB APB1 clock
  //
  rcu_periph_clock_enable(RCU_USBD);


  // configure usb pull-up pin
  //
  gpio_init(USB_PULLUP, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, USB_PULLUP_PIN);


#ifdef _USE_HW_CLI
  cliAdd("usb", cliCmd);
#endif
  return ret;
}

void usbDeInit(void)
{
  if (is_init == true)
  {

  }
}

bool usbIsOpen(void)
{
  return cdcIsConnect();
}

bool usbIsConnect(void)
{

  return true;
}

UsbMode_t usbGetMode(void)
{
  return is_usb_mode;
}

UsbType_t usbGetType(void)
{
  return (UsbType_t)cdcGetType();
}

bool usbBegin(UsbMode_t usb_mode)
{
  bool ret = false;


#if HW_USE_CDC == 1

  if (usb_mode == USB_CDC_MODE)
  {
    // USB device configuration
    //
    usbd_init(&usbd_cdc, &cdc_desc, &cdc_class);


    // 2 bits for preemption priority, 2 bits for subpriority
    //
    nvic_priority_group_set(NVIC_PRIGROUP_PRE2_SUB2);

    // enable the USB low priority interrupt
    //
    nvic_irq_enable((uint8_t)USBD_LP_CAN0_RX0_IRQn, 2U, 0U);

    // enable the USB low priority interrupt
    //
    nvic_irq_enable((uint8_t)USBD_HP_CAN0_TX_IRQn, 1U, 0U);


    // enabled USB pull-up
    //
    usbd_connect(&usbd_cdc);


    cdcInit();

    is_usb_mode = USB_CDC_MODE;
    ret = true;
  }
#endif

#if HW_USE_MSC == 1

  if (usb_mode == USB_MSC_MODE)
  {
    if (USBD_Init(&hUsbDeviceFS, &MSC_Desc, DEVICE_FS) != USBD_OK)
    {
      return false;
    }
    if (USBD_RegisterClass(&hUsbDeviceFS, &USBD_MSC) != USBD_OK)
    {
      return false;
    }
    if (USBD_MSC_RegisterStorage(&hUsbDeviceFS, &USBD_Storage_Interface_fops_FS) != USBD_OK)
    {
      return false;
    }
    if (USBD_Start(&hUsbDeviceFS) != USBD_OK)
    {
      return false;
    }

    is_usb_mode = USB_MSC_MODE;
    ret = true;
  }
#endif


  is_init = ret;

  return ret;
}

void USBD_LP_CAN0_RX0_IRQHandler (void)
{
    usbd_isr();
}

#ifdef _USE_HW_CLI
void cliCmd(cli_args_t *args)
{
  bool ret = false;

  if (args->argc == 1 && args->isStr(0, "info") == true)
  {
    while(cliKeepLoop())
    {
      cliPrintf("USB Connect : %d\n", usbIsConnect());
      cliPrintf("USB Open    : %d\n", usbIsOpen());
      cliPrintf("\x1B[%dA", 2);
      delay(100);
    }
    cliPrintf("\x1B[%dB", 2);

    ret = true;
  }

  if (args->argc == 1 && args->isStr(0, "tx") == true)
  {
    uint32_t pre_time;
    uint32_t tx_cnt = 0;

    while(cliKeepLoop())
    {
      if (millis()-pre_time >= 1000)
      {
        pre_time = millis();
        logPrintf("tx : %d KB/s\n", (int)tx_cnt/1024);
        tx_cnt = 0;
      }
      cdcWrite((uint8_t *)"123456789012345678901234567890\n", 31);
      tx_cnt += 31;
    }
    cliPrintf("\x1B[%dB", 2);

    ret = true;
  }

  if (args->argc == 1 && args->isStr(0, "rx") == true)
  {
    uint32_t pre_time;
    uint32_t rx_cnt = 0;
    uint32_t rx_len;

    while(cliKeepLoop())
    {
      if (millis()-pre_time >= 1000)
      {
        pre_time = millis();
        logPrintf("rx : %d KB/s\n", (int)rx_cnt/1024);
        rx_cnt = 0;
      }

      rx_len = cdcAvailable();

      for (int i=0; i<rx_len; i++)
      {
        cdcRead();
      }

      rx_cnt += rx_len;
    }
    cliPrintf("\x1B[%dB", 2);

    ret = true;
  }

  if (ret == false)
  {
    cliPrintf("usb info\n");
    cliPrintf("usb tx\n");
    cliPrintf("usb rx\n");
  }
}
#endif


#endif
