<script lang="ts" setup>
import { Platform, HomeFilled, Setting, User, UserFilled, Monitor, Connection, Document, Menu } from '@element-plus/icons-vue'
import { useRouter, useRoute } from 'vue-router'
import { defineProps, computed } from 'vue'

defineProps({
  collapse: {
    type: Boolean,
    default: false
  }
})

const router = useRouter()

const handleSelect = (key: string) => {
  router.push({ name: key })
}

const route = useRoute()

// 根据路径动态设置当前激活的菜单项
// 需要搭配路由配置的 name 属性一起使用
const routeName = computed(() => {
  const lastSlashIndex = route.path.lastIndexOf('/')
  if (lastSlashIndex === -1)
    return route.path
  return route.path.slice(lastSlashIndex + 1)
})
</script>

<template>
  <div class="logo">
    <el-icon class="logo-icon" :size="32"><Platform /></el-icon>
    <span v-show="!collapse" class="logo-text">后台管理系统</span>
  </div>
  <el-scrollbar>
    <el-menu
        :default-active="routeName"
        class="el-menu-vertical"
        :collapse="collapse"
        @select="handleSelect"
        background-color="#001529"
        text-color="#a3b3cc"
        active-text-color="#fff"
        :unique-opened="true"
    >
      <el-menu-item index="home">
        <el-icon><HomeFilled /></el-icon>
        <template #title>首页</template>
      </el-menu-item>

      <el-sub-menu index="auth">
        <template #title>
          <el-icon><Setting /></el-icon>
          <span>系统管理</span>
        </template>
        <el-menu-item index="user">
          <el-icon><User /></el-icon>
          <template #title>用户管理</template>
        </el-menu-item>
        <el-menu-item index="roles">
          <el-icon><UserFilled /></el-icon>
          <template #title>角色管理</template>
        </el-menu-item>
        <el-menu-item index="menus">
          <el-icon><Menu /></el-icon>
          <template #title>菜单管理</template>
        </el-menu-item>
      </el-sub-menu>

      <el-sub-menu index="monitor">
        <template #title>
          <el-icon><Monitor /></el-icon>
          <span>系统监控</span>
        </template>
        <el-menu-item index="online">
          <el-icon><Connection /></el-icon>
          <template #title>在线用户</template>
        </el-menu-item>
        <el-menu-item index="logs">
          <el-icon><Document /></el-icon>
          <template #title>操作日志</template>
        </el-menu-item>
      </el-sub-menu>
    </el-menu>
  </el-scrollbar>
</template>

<style lang="scss" scoped>
.logo {
  height: 60px;
  display: flex;
  align-items: center;
  padding: 0 20px;
  background: var(--menu-logo);
  overflow: hidden;
  .logo-icon {
    color: #40a9ff;
    margin-right: 12px;
    transition: all 0.3s;
    filter: drop-shadow(0 0 8px rgba(64,169,255,.5));
    &:hover {
      transform: rotate(360deg);
      color: #69c0ff;
    }
  }
  .logo-text {
    color: #fff;
    font-size: 18px;
    font-weight: 600;
    white-space: nowrap;
    background: linear-gradient(to right, #40a9ff, #36cfc9);
    background-clip: text;
    -webkit-background-clip: text;
    -webkit-text-fill-color: transparent;
  }
}

.el-menu-vertical {
  border-right: none;
  background-color: transparent !important;
  height: calc(100vh - 60px);
  &:not(.el-menu--collapse) {
    width: 220px;
  }
}

:deep(.el-menu-item), :deep(.el-sub-menu__title) {
  height: 50px;
  line-height: 50px;
  margin: 4px 0;
  border-radius: 0;
  color: var(--menu-text) !important;
  padding: 0 20px !important;

  &:hover {
    color: var(--menu-active-text) !important;
    background: var(--menu-hover) !important;
  }

  .el-icon {
    color: inherit;
  }
}

:deep(.el-menu-item.is-active) {
  color: var(--menu-active-text) !important;
  background: var(--primary-color) !important;
  box-shadow: 0 4px 12px rgba(24,144,255,.3);

  .el-icon {
    color: #fff;
  }
}

:deep(.el-sub-menu .el-menu-item) {
  padding-left: 48px !important;
}

:deep(.el-sub-menu.is-opened) {
  .el-sub-menu__title {
    color: #fff !important;
  }
}
</style>
