<script setup lang="ts">
import mqtt, { type MqttClient } from 'mqtt'

const client = ref<MqttClient | null>(null)

enum Topics {
  Events = 'events',
  Commands = 'commands',
}

onMounted(() => {
  client.value = mqtt.connect('mqtt://192.168.41.60:8080')

  client.value.on('connect', () => client.value?.subscribe(Topics.Events))

  client.value.on('message', (topic, message) => {
    // message is Buffer
    const args = message.toString().split(' ')
    humidity.value = parseFloat(args[0])
    temperature.value = parseFloat(args[1])
    ldr.value = args[2] === '1'
    control_led.value = args[3] === '1'
    // buzzer.value = args[4] === '1'
    console.log(message.toString())
  })
})

const ldr = ref(false)
const control_led = ref(false)
const humidity = ref(0.0)
const temperature = ref(0.0)
const buzzer = ref(false)

const send = (type: 'control_led' | 'buzzer') => {
  switch (type) {
    case 'control_led':
      client.value?.publish(
        Topics.Commands,
        `${buzzer.value ? '1' : '0'}${control_led.value ? '0' : '1'}`
      )
      break
    case 'buzzer':
      buzzer.value = !buzzer.value
      client.value?.publish(
        Topics.Commands,
        `${buzzer.value ? '1' : '0'}${control_led.value ? '1' : '0'}`
      )
      break
  }
}
</script>
<template>
  <div class="mt-12 container">
    <h1 class="scroll-m-20 mb-10 text-5xl font-bold tracking-tight">
      Super-duper Dashboard <span class="text-xs">we are all gonna die</span>
    </h1>
    <div class="grid gap-8 sm:grid-cols-2 lg:grid-cols-3">
      <UiCard>
        <UiCardHeader>
          <UiCardTitle class="leading-9">Humidity</UiCardTitle>
        </UiCardHeader>
        <UiCardContent class="flex gap-2">
          <span class="text-2xl font-bold"> {{ humidity }}% </span>
        </UiCardContent>
      </UiCard>
      <UiCard>
        <UiCardHeader>
          <UiCardTitle class="leading-9">Temperature</UiCardTitle>
        </UiCardHeader>
        <UiCardContent class="flex gap-2">
          <span class="text-2xl font-bold"> {{ temperature }}ºC </span>
        </UiCardContent>
      </UiCard>
      <UiCard>
        <UiCardHeader>
          <UiCardTitle class="leading-9">LDR</UiCardTitle>
        </UiCardHeader>
        <UiCardContent class="flex gap-2">
          <UiToggle disabled class="flex-1" variant="outline" :pressed="ldr">
            {{ ldr ? 'On' : 'Off' }}
          </UiToggle>
          <div class="size-10 *:m-auto content-center">
            <IconsLightBulbFilled v-if="ldr" />
            <IconsLightBulb v-else />
          </div>
        </UiCardContent>
      </UiCard>
      <UiCard>
        <UiCardHeader>
          <UiCardTitle class="flex-1 leading-9">Control LED</UiCardTitle>
        </UiCardHeader>

        <UiCardContent class="space-y-4">
          <div class="flex gap-2">
            <UiButton
              class="flex-1"
              variant="outline"
              @click="() => send('control_led')"
            >
              {{ control_led ? 'On' : 'Off' }}
              {{ control_led }}
            </UiButton>

            <div class="size-10 *:m-auto content-center">
              <IconsLightBulbFilled v-if="control_led" />
              <IconsLightBulb v-else />
            </div>
          </div>
        </UiCardContent>
      </UiCard>
      <UiCard>
        <UiCardHeader>
          <UiCardTitle class="flex-1 leading-9">Buzzer</UiCardTitle>
        </UiCardHeader>

        <UiCardContent class="space-y-4">
          <div class="flex gap-2">
            <UiButton
              class="flex-1"
              variant="outline"
              @mousedown="() => send('buzzer')"
              @mouseup="() => send('buzzer')"
            >
              Ring
              {{ buzzer }}
            </UiButton>

            <div class="size-10 *:m-auto content-center">
              <IconsVolume v-if="buzzer" />
              <IconsVolumeOff v-else />
            </div>
          </div>
        </UiCardContent>
      </UiCard>
      <div class="space-y-4">
        <UiButton
          variant="ghost"
          class="w-full space-x-4"
          @click="() => client?.publish(Topics.Commands, '00')"
        >
          <span class="mr-2">
            <IconsBiohazard :size="16" />
          </span>
          panik</UiButton
        >
        <UiButton
          class="w-full"
          variant="ghost"
          @click="() => client?.publish(Topics.Commands, '11')"
        >
          <span class="mr-2"> <IconsParty :size="16" /> </span>party</UiButton
        >
      </div>
    </div>
  </div>

  <!-- <Esp32 /> -->
</template>
