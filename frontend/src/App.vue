<script setup lang="ts">
import zhCN from 'element-plus/dist/locale/zh-cn.mjs'
import { ElConfigProvider } from 'element-plus'
</script>

<template>
  <el-config-provider :locale="zhCN">
    <RouterView />
  </el-config-provider>
</template>

<style scoped lang="scss">
</style>
