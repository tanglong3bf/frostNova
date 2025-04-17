<script setup lang="ts">
import { ref, onMounted } from 'vue'
import { useThemeStore } from '@/stores/theme'
import { themes } from '@/styles/themes'
import AsideMenuComponent from "@/layouts/components/AsideMenu.vue";
import HeaderComponent from "@/layouts/components/Header.vue";

const isCollapse = ref(false)

const themeStore = useThemeStore()

onMounted(() => {
  themeStore.setTheme(themes[0])
})

</script>

<template>
  <el-container class="layout-container">
    <el-aside :width="isCollapse ? '64px' : '220px'" class="aside">
      <AsideMenuComponent :collapse="isCollapse" />
    </el-aside>
    <el-container>
      <el-header class="header">
        <HeaderComponent :aside-collapse="isCollapse" @toggle-aside-collapse="isCollapse = !isCollapse" />
      </el-header>
      <el-main>
        <router-view/>
      </el-main>
    </el-container>
  </el-container>
</template>

<style>
:root {
  --menu-bg: #001529;
  --menu-text: #a3b3cc;
  --menu-active-text: #fff;
  --menu-hover: rgba(255,255,255,.1);
  --menu-logo: linear-gradient(to right, #001529, #003a70);
  --primary-color: #1890ff;
  --primary-hover: #40a9ff;
  --primary-light: #f0f7ff;
  transition: all 0.3s ease-in-out;
}

.aside,
.logo,
:deep(.el-menu-item),
:deep(.el-sub-menu__title),
:deep(.el-menu-item.is-active) {
  transition: all 0.3s ease-in-out;
}
</style>
<style scoped>
.layout-container {
  height: 100vh;
  background-color: #f6f8f9;
}

.aside {
  background-color: var(--menu-bg);
  transition: width 0.3s;
  overflow: hidden;
  box-shadow: 2px 0 8px rgba(0,21,41,.15);
  flex-shrink: 0;
}

.header {
  display: flex;
  align-items: center;
  justify-content: space-between;
  background-color: #fff;
  box-shadow: 0 1px 4px rgba(0,21,41,.08);
  padding: 0 16px;
  height: 56px;
  position: relative;
  z-index: 100;
  flex-shrink: 0;
}
</style>
